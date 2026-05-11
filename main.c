#include <stdio.h>
#include "NuMicro.h"
#include "EBI_LCD_Module.h"   // ILI9341_Initial(), TimerDelay_*, Timer3_Init()
#include "game_draw.h"        // StartGameField(), DisplayGameField(), GameOverField()
#include "playfield.h"        // ClearPlayfield(), CanPlaceTetromino()
#include "spawn_block.h"      // InitSpawn(), SpawnTetromino()
#include "rotation.h"         // RotateTetromino*()
#include "tetris_draw.h"      // DrawTetromino(), LockTetromino(), RedrawPlayfield()
/* how many pixels per step */
#define STEP_Y 10
extern volatile uint8_t Timer3_flag;

/* button-edge trackers */
static uint8_t prev_PA0  = 1;  // SW1 (start/pause)
static uint8_t prev_PA1  = 1;  // SW2 (restart/manual drop)
static uint8_t prev_PG2  = 1;  // up = rotate CW
static uint8_t prev_PG4  = 1;  // right
static uint8_t prev_PC9  = 1;  // left
static uint8_t prev_PC10 = 1;  // down = hard drop



/* accumulate ticks between drops */
static uint8_t gravity_counter = 0;

/* game state flags */
static uint8_t game_started = 0;
static uint8_t isPaused = 0;
static uint8_t game_over = 0;  // game over flag

int32_t main(void)
{
    TetrominoType tetromino = TETROMINO_I;
    char acString[32];
    uint8_t spawnNew = 0;
    uint16_t originX = 0, originY = 0; 
    uint8_t rotation = 0;
		uint8_t spawn_delay_active = 0;     // <-- new
		uint8_t spawn_counter = 0;          // <-- new
		uint8_t spawn_ticks_per_spawn = 0;  // <-- new
    uint8_t cur_PA0, cur_PA1, cur_PG2, cur_PG4, cur_PC9, cur_PC10;
    uint8_t ticks_per_drop;
    float drop_interval;

    /* 1) Basic init */
    SYS_UnlockReg();
    SYS_Init();
    UART_Open(UART0, 115200);

    /* 2) LCD over EBI */
    EBI_Open(EBI_BANK0, EBI_BUSWIDTH_16BIT,
             EBI_TIMING_NORMAL, 0, EBI_CS_ACTIVE_LOW);
    ILI9341_Initial();

    /* 3) 100 ms heartbeat + delay infrastructure */
    Timer3_Init();
    __enable_irq();

    /* 4) Turn on backlight */
    GPIO_SetMode(PB, BIT7, GPIO_MODE_OUTPUT);
    PB7 = 1;

    /* 5) Show splash */
    StartGameField();

    /* 6) Configure SW1 = PA0 for press to start */
    GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);
    PA->DBEN  |= BIT0;
    PA->PUSEL |= BIT0;
    PA0 = 0;            // force line low to enable internal pull-up
    prev_PA0 = PA0;
    TimerDelay_Start(0); // clear any delay

    /* 7) Wait here until SW1 goes from high->low, then release */
    while (1)
    {
        if (Timer3_flag) Timer3_flag = 0;
        if (!TimerDelay_Done()) continue;

        cur_PA0 = PA0;
        if (prev_PA0 && !cur_PA0)
        {
            /* debounce */
            TimerDelay_Start(2);
            /* wait for release */
            do {
                if (Timer3_flag) Timer3_flag = 0;
            } while (!PA0);
            prev_PA0 = 1;
            break;
        }
        prev_PA0 = cur_PA0;
    }

    /* 8) Clear splash and draw the full game field */
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    DisplayGameField();
    game_started  = 1;
    timer_running = 1;  // start the play clock 

    /* Initialize score and level display */
    currentScore = 0;
    level = 1;
    sprintf(acString, "%d", currentScore);
    LCD_PutString(176, 150, (uint8_t*)acString, C_WHITE, C_BLACK);
    sprintf(acString, "%d", level);
    LCD_PutString(176, 210, (uint8_t*)acString, C_WHITE, C_BLACK);

    /* 9) Seed & draw first next block */
    InitSpawn();
		/* 1) Activate spawn delay */
		spawn_delay_active = 1;
		/* 2) Compute spawn delay in 100 ms ticks */
		spawn_ticks_per_spawn = (uint8_t)((0.5f - (level - 1) * 0.05f) / 0.1f + 0.5f);
		if (spawn_ticks_per_spawn < 1)
				spawn_ticks_per_spawn = 1;
		/* 3) Reset counter */
		spawn_counter = 0;

    /* 10) Clear logical grid */
    ClearPlayfield();

    /* 11) Configure all controls for the game loop */
    GPIO_SetMode(PG, BIT2|BIT3|BIT4, GPIO_MODE_INPUT);
    GPIO_SetMode(PC, BIT9|BIT10,      GPIO_MODE_INPUT);
    GPIO_SetMode(PA, BIT0|BIT1,       GPIO_MODE_INPUT);
    PG->DBEN |= BIT2|BIT3|BIT4;
    PC->DBEN |= BIT9|BIT10;
    PA->DBEN |= BIT0|BIT1;
    PG->PUSEL |= BIT2|BIT4;
    PC->PUSEL |= BIT9|BIT10;
    PA->PUSEL |= BIT0|BIT1;
    PG2 = PG3 = PG4 = PC9 = PC10 = PA0 = PA1 = 0; // pull-ups

    /* 12) Main Tetris loop */
    while (1)
    {
        if (Timer3_flag) Timer3_flag = 0;

        if (TimerDelay_Done())
        {
            /* Read current button states */
            cur_PA0  = PA0;
            cur_PA1  = PA1;
            cur_PG2  = PG2;
            cur_PG4  = PG4;
            cur_PC9  = PC9;
            cur_PC10 = PC10;

            /* --- GAME OVER HANDLING --- */
            if (game_over)
            {
								    if (prev_PA0 && !cur_PA0) {
											LeaderBoard();
											TimerDelay_Start(2);
										}
										prev_PA0 = cur_PA0;

                /* Only respond to restart (SW2 = PA1) */
                if (prev_PA1 && !cur_PA1)
                {
                    /* Reset game state */
                    game_over      = 0;
                    isPaused       = 0;
                    currentScore   = 0;
                    level          = 1;
                    gravity_counter = 0;

                    /* Reset timer */
                    timer_running  = 1;
                    minutes        = 0;
                    seconds        = 0;

                    /* Clear and redraw the playfield */
                    ClearPlayfield();
                    DisplayGameField();

                    /* Re-spawn first block */
                    InitSpawn();
                    /* 1) Activate spawn delay */
										spawn_delay_active = 1;
										/* 2) Compute spawn delay in 100 ms ticks */
										spawn_ticks_per_spawn = (uint8_t)((0.5f - (level - 1) * 0.05f) / 0.1f + 0.5f);
										if (spawn_ticks_per_spawn < 1)
												spawn_ticks_per_spawn = 1;
										/* 3) Reset counter */
										spawn_counter = 0;

                    /* Reset score display */
                    sprintf(acString, "%d", currentScore);
                    LCD_PutString(176, 150, (uint8_t*)acString, C_WHITE, C_BLACK);

                    /* Reset level display */
                    sprintf(acString, "%d", level);
                    LCD_PutString(176, 210, (uint8_t*)acString, C_WHITE, C_BLACK);

                    /* Reset timer display */
                    sprintf(acString, "%02d:%02d", minutes, seconds);
                    LCD_PutString(176, 180, (uint8_t*)acString, C_WHITE, C_BLACK);

                    LCD_PutString(176, 260, (uint8_t*)"PAUSE ", C_WHITE, C_BLACK);
                    TimerDelay_Start(2);
                }
                prev_PA1 = cur_PA1;
                /* Skip other input processing */
                continue;
            }
            /* --- NORMAL INPUT --- */

            /* PA0 = Pause toggle */
						/* PA0 = Pause toggle */
						if (prev_PA0 && !cur_PA0)
						{
								isPaused = !isPaused;
								/* stop/start the play-clock */
								timer_running = isPaused ? 0 : 1;
								if (isPaused)
								{
										LCD_PutString(176,260,(uint8_t*)"PAUSED",C_RED,C_BLACK);
								}
								else
								{
										LCD_PutString(176,260,(uint8_t*)"PAUSE ",C_WHITE,C_BLACK);
								}
								TimerDelay_Start(2);
						}
						prev_PA0 = cur_PA0;
						// Display the timer (mm:ss)
            sprintf(acString, "%02d:%02d", minutes, seconds);
            LCD_PutString(176, 180, (uint8_t*)acString, C_WHITE, C_BLACK);  // Positioned under SCORE and LEVEL
            /* PA1 = Restart if paused else manual drop */
						if (prev_PA1 && !cur_PA1)
						{
								if (isPaused)
								{
										/* 1) Clear & redraw field */
										LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
										ClearPlayfield();
										DisplayGameField();
										InitSpawn();
										/* 1) Activate spawn delay */
										spawn_delay_active = 1;
										/* 2) Compute spawn delay in 100 ms ticks */
										spawn_ticks_per_spawn = (uint8_t)((0.5f - (level - 1) * 0.05f) / 0.1f + 0.5f);
										if (spawn_ticks_per_spawn < 1)
												spawn_ticks_per_spawn = 1;
										/* 3) Reset counter */
										spawn_counter = 0;
											isPaused = 0;

										/* 2) Reset and restart the timer */
										timer_running = 1;
										minutes = 0;
										seconds = 0;
										sprintf(acString, "%02d:%02d", minutes, seconds);
										LCD_PutString(176, 180, (uint8_t*)acString, C_WHITE, C_BLACK);

										/* 3) Reset score and level, then redraw them */
										currentScore = 0;
										level        = 1;

										sprintf(acString, "%d", currentScore);
										LCD_PutString(176, 150, (uint8_t*)acString, C_WHITE, C_BLACK);

										sprintf(acString, "%d", level);
										LCD_PutString(176, 215, (uint8_t*)acString, C_WHITE, C_BLACK);

										/* 4) Restore the PAUSED label back to PAUSE  */
										LCD_PutString(176, 260, (uint8_t*)"PAUSE ", C_WHITE, C_BLACK);
								}
								else
								{
										/* Manual drop tick */
										Timer3_flag = 1;
								}
								TimerDelay_Start(2);
						}
						prev_PA1 = cur_PA1;
            /* Rotate CW */
            if (!isPaused && prev_PG2 && !cur_PG2)
            {
                RotateTetrominoClockwise(tetromino,&originX,&originY,&rotation);
                TimerDelay_Start(2);
            }
            prev_PG2 = cur_PG2;

            /* Move right */
            if (!isPaused && prev_PG4 && !cur_PG4)
            {
                if (CanMoveRight(tetromino,originX,originY,rotation))
                {
                    ClearTetromino(tetromino,originX,originY,rotation);
                    originX += 10;
                    DrawTetromino(tetromino,originX,originY,rotation);
                }
                TimerDelay_Start(2);
            }
            prev_PG4 = cur_PG4;

            /* Move left */
            if (!isPaused && prev_PC9 && !cur_PC9)
            {
                if (CanMoveLeft(tetromino,originX,originY,rotation))
                {
                    ClearTetromino(tetromino,originX,originY,rotation);
                    originX -= 10;
                    DrawTetromino(tetromino,originX,originY,rotation);
                }
                TimerDelay_Start(2);
            }
            prev_PC9 = cur_PC9;

            /* Hard-drop on PC10 */
            if (!isPaused && prev_PC10 && !cur_PC10)
            {
                ClearTetromino(tetromino, originX, originY, rotation);
                while (CanMoveDown(tetromino, originX, originY, rotation))
                    originY += 10;
                DrawTetromino(tetromino, originX, originY, rotation);
                LockTetromino(tetromino, originX, originY, rotation);

                /* Clear lines and update score */
                {
                    int lines_cleared = ClearFullLines();
                    if (lines_cleared > 0)
                    {
                        currentScore += lines_cleared;

                        /* Increase level every 5 points */
                        uint8_t new_level = currentScore / 5 + 1;
                        if (new_level > 10)
                            new_level = 10;

                        if (new_level != level)
                        {
                            level = new_level;
                        }

                        /* Update score display */
                        sprintf(acString, "%d", currentScore);
                        LCD_PutString(176, 150, (uint8_t*)acString, C_WHITE, C_BLACK);

                        /* Update level display */
                        sprintf(acString, "%d", level);
                        LCD_PutString(176, 210, (uint8_t*)acString, C_WHITE, C_BLACK);
                    }
                }

								 /* 1) Activate spawn delay */
								spawn_delay_active = 1;
								/* 2) Compute spawn delay in 100 ms ticks */
								spawn_ticks_per_spawn = (uint8_t)((0.5f - (level - 1) * 0.05f) / 0.1f + 0.5f);
								if (spawn_ticks_per_spawn < 1)
										spawn_ticks_per_spawn = 1;
								/* 3) Reset counter */
								spawn_counter = 0;
                TimerDelay_Start(2);
            }
            prev_PC10 = cur_PC10;
        }

/* --- GRAVITY TICK --- */
					if (Timer3_flag && !isPaused && !game_over)
					{
							Timer3_flag = 0;

							/* Compute drop interval for current level */
							drop_interval = 0.5f - (level - 1) * 0.05f;
							ticks_per_drop = (uint8_t)(drop_interval / 0.1f + 0.5f);
							if (ticks_per_drop < 1)
									ticks_per_drop = 1;

							gravity_counter++;

							if (gravity_counter >= ticks_per_drop)
							{
									gravity_counter = 0;

									/* --- 1) Spawn-delay countdown --- */
									if (spawn_delay_active)
									{
											spawn_counter++;
											if (spawn_counter >= spawn_ticks_per_spawn)
											{
													spawn_delay_active = 0;
													spawn_counter = 0;
													spawnNew = 1;
											}
									}
									/* --- 2) Actually spawn the next block --- */
									else if (spawnNew)
									{
											tetromino = SpawnTetromino(&originX, &originY);
											rotation  = 0;

											/* GAME OVER CHECK */
											if (!CanPlaceTetromino(tetromino, originX, originY, rotation))
											{
													game_over = 1;
													isPaused  = 1;
													AddToLeaderboard(currentScore, level, minutes, seconds);
													GameOverField();
											}
											else
											{
													spawnNew = 0;
											}
									}
									/* --- 3) Normal gravity: move down or lock and schedule next spawn --- */
									else
									{
											ClearTetromino(tetromino, originX, originY, rotation);
											if (CanMoveDown(tetromino, originX, originY, rotation))
											{
													originY += 10;
													DrawTetromino(tetromino, originX, originY, rotation);
											}
											else
											{
													DrawTetromino(tetromino, originX, originY, rotation);
													LockTetromino(tetromino, originX, originY, rotation);

													/* Clear lines and update score/level */
													{
															int lines_cleared = ClearFullLines();
															if (lines_cleared > 0)
															{
																	currentScore += lines_cleared;
																	{
																			uint8_t new_level = currentScore / 5 + 1;
																			if (new_level > 10) new_level = 10;
																			if (new_level != level) level = new_level;
																	}
																	/* redraw score */
																	sprintf(acString, "%d", currentScore);
																	LCD_PutString(176,150,(uint8_t*)acString,C_WHITE,C_BLACK);
																	/* redraw level */
																	sprintf(acString, "%d", level);
																	LCD_PutString(176,210,(uint8_t*)acString,C_WHITE,C_BLACK);
															}
													}

													/* Schedule next spawn after delay */
													spawn_delay_active     = 1;
													spawn_ticks_per_spawn  = (uint8_t)((0.5f - (level - 1) * 0.05f) / 0.1f + 0.5f);
													if (spawn_ticks_per_spawn < 1)
															spawn_ticks_per_spawn = 1;
													spawn_counter = 0;
											}
									}
							}
					}
							}

							/* Never reached */
							return 0;
					}
