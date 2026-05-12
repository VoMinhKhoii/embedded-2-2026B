#include <stdio.h>
#include "NuMicro.h"
#include "EBI_LCD_Module.h"
#include "game_screens.h"
#include "game_session.h"
#include "playfield.h"
#include "tetromino_rotation.h"
#include "tetris_draw.h"

extern volatile uint8_t Timer3_flag;

typedef struct {
    /* Previous sampled button states for edge-triggered input. */
    uint8_t prev_PA0;
    uint8_t prev_PA1;
    uint8_t prev_PG2;
    uint8_t prev_PG4;
    uint8_t prev_PC9;
    uint8_t prev_PC10;
} ButtonState;

static ButtonState g_buttons = {1, 1, 1, 1, 1, 1};

static uint8_t GetTicksForLevel(void)
{
    /* Drop speed increases with level, but never faster than 1 tick. */
    float interval = 0.5f - (level - 1) * 0.05f;
    uint8_t ticks = (uint8_t)(interval / 0.1f + 0.5f);

    if (ticks < 1) {
        ticks = 1;
    }

    return ticks;
}

static void ScheduleSpawnDelay(GameSession *session)
{
    /* Reuse the level-based timing for the post-lock spawn delay. */
    session->spawn_delay_active = 1;
    session->spawn_ticks_per_spawn = GetTicksForLevel();
    session->spawn_counter = 0;
}

static void ClearHudField(uint16_t x, uint16_t y, uint16_t w)
{
    LCD_BlankArea(x, y, w, 16, RIGHT_PANEL_BG_COLOR);
}

static void UpdateScoreDisplay(void)
{
    char text[32];

    ClearHudField(167, 120, 48);
    sprintf(text, "%d", currentScore);
    LCD_PutString(167, 120, (uint8_t *)text, 0x01AF, RIGHT_PANEL_BG_COLOR);
}

static void UpdateLevelDisplay(void)
{
    char text[32];

    ClearHudField(167, 195, 48);
    sprintf(text, "%d", level);
    LCD_PutString(167, 195, (uint8_t *)text, 0x01AF, RIGHT_PANEL_BG_COLOR);
}

static void UpdateTimerDisplay(void)
{
    char text[32];

    ClearHudField(167, 155, 56);
    sprintf(text, "%02d:%02d", minutes, seconds);
    LCD_PutString(167, 155, (uint8_t *)text, 0x01AF, RIGHT_PANEL_BG_COLOR);
}

static void UpdateScoreAndLevel(int lines_cleared)
{
    uint8_t new_level;

    if (lines_cleared <= 0) {
        return;
    }

    currentScore += (uint16_t)lines_cleared;
    new_level = currentScore / 5 + 1;
    if (new_level > 10) {
        new_level = 10;
    }
    level = new_level;

    UpdateScoreDisplay();
    UpdateLevelDisplay();
}

static void ResetGameStats(GameSession *session)
{
    /* Reset the current run without touching the leaderboard. */
    currentScore = 0;
    level = 1;
    minutes = 0;
    seconds = 0;
    timer_running = 1;

    session->gravity_counter = 0;
    session->isPaused = 0;
    session->game_over = 0;
    session->rotation = 0;
    session->spawnNew = 0;

    UpdateScoreDisplay();
    UpdateLevelDisplay();
    UpdateTimerDisplay();
    ClearHudField(167, 235, 56);
    LCD_PutString(167, 235, (uint8_t *)"PAUSE ", 0x01AF, RIGHT_PANEL_BG_COLOR);
}

static void ResetPlayfieldAndPreview(GameSession *session)
{
    /* Redraw the board and prepare the next-piece preview. */
    ClearPlayfield();
    DrawGameplayScreen();
    InitializeTetrominoQueue();
    ScheduleSpawnDelay(session);
}

static void RestartGame(GameSession *session)
{
    ResetPlayfieldAndPreview(session);
    ResetGameStats(session);
}

static void LockActiveTetromino(GameSession *session)
{
    int lines_cleared;

    /* Convert the active piece into playfield state, then score clears. */
    LockTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    lines_cleared = ClearFullLines();
    UpdateScoreAndLevel(lines_cleared);
    ScheduleSpawnDelay(session);
}

static void HandleGameOver(GameSession *session)
{
    /* Freeze gameplay and persist this run in the in-memory leaderboard. */
    session->game_over = 1;
    session->isPaused = 1;
    RecordLeaderboardEntry(currentScore, level, minutes, seconds);
    DrawGameOverScreen();
}

static void TogglePause(GameSession *session)
{
    /* Pausing also stops the elapsed game timer. */
    session->isPaused = !session->isPaused;
    timer_running = session->isPaused ? 0 : 1;

    ClearHudField(167, 235, 56);
    if (session->isPaused) {
        LCD_PutString(167, 235, (uint8_t *)"PAUSED", C_RED, RIGHT_PANEL_BG_COLOR);
    } else {
        LCD_PutString(167, 235, (uint8_t *)"PAUSE ", 0x01AF, RIGHT_PANEL_BG_COLOR);
    }
}

static void HandleGameOverInput(GameSession *session, uint8_t cur_PA0, uint8_t cur_PA1)
{
    if (g_buttons.prev_PA0 && !cur_PA0) {
        DrawLeaderboardScreen();
        TimerDelay_Start(2);
    }
    g_buttons.prev_PA0 = cur_PA0;

    if (g_buttons.prev_PA1 && !cur_PA1) {
        RestartGame(session);
        TimerDelay_Start(2);
    }
    g_buttons.prev_PA1 = cur_PA1;
}

static void HandlePauseOrManualDrop(GameSession *session, uint8_t cur_PA1)
{
    if (!(g_buttons.prev_PA1 && !cur_PA1)) {
        g_buttons.prev_PA1 = cur_PA1;
        return;
    }

    /* SW2 restarts when paused; otherwise it forces one gravity tick. */
    if (session->isPaused) {
        RestartGame(session);
    } else {
        Timer3_flag = 1;
    }

    TimerDelay_Start(2);
    g_buttons.prev_PA1 = cur_PA1;
}

static void HandleHorizontalMove(GameSession *session, uint8_t move_right)
{
    uint8_t can_move = move_right
        ? CanMoveRight(session->tetromino, session->originX, session->originY, session->rotation)
        : CanMoveLeft(session->tetromino, session->originX, session->originY, session->rotation);

    if (!can_move) {
        return;
    }

    /* Redraw the piece at its new column after a successful move. */
    ClearTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    if (move_right) {
        session->originX += STONE_BLOCK;
    } else {
        session->originX -= STONE_BLOCK;
    }
    DrawTetromino(session->tetromino, session->originX, session->originY, session->rotation);
}

static void HandleHardDrop(GameSession *session)
{
    /* Drop until collision, then lock immediately. */
    ClearTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    while (CanMoveDown(session->tetromino, session->originX, session->originY, session->rotation)) {
        session->originY += STONE_BLOCK;
    }
    DrawTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    LockActiveTetromino(session);
    TimerDelay_Start(2);
}

static void HandleNormalInput(GameSession *session,
                              uint8_t cur_PA0,
                              uint8_t cur_PA1,
                              uint8_t cur_PG2,
                              uint8_t cur_PG4,
                              uint8_t cur_PC9,
                              uint8_t cur_PC10)
{
    /* Each control is edge-triggered to avoid repeated moves per poll. */
    if (g_buttons.prev_PA0 && !cur_PA0) {
        TogglePause(session);
        TimerDelay_Start(2);
    }
    g_buttons.prev_PA0 = cur_PA0;

    UpdateTimerDisplay();
    HandlePauseOrManualDrop(session, cur_PA1);

    if (!session->isPaused && g_buttons.prev_PG2 && !cur_PG2) {
        RotateTetrominoClockwiseWithKick(session->tetromino,
                                         &session->originX,
                                         &session->originY,
                                         &session->rotation);
        TimerDelay_Start(2);
    }
    g_buttons.prev_PG2 = cur_PG2;

    if (!session->isPaused && g_buttons.prev_PG4 && !cur_PG4) {
        HandleHorizontalMove(session, 1);
        TimerDelay_Start(2);
    }
    g_buttons.prev_PG4 = cur_PG4;

    if (!session->isPaused && g_buttons.prev_PC9 && !cur_PC9) {
        HandleHorizontalMove(session, 0);
        TimerDelay_Start(2);
    }
    g_buttons.prev_PC9 = cur_PC9;

    if (!session->isPaused && g_buttons.prev_PC10 && !cur_PC10) {
        HandleHardDrop(session);
    }
    g_buttons.prev_PC10 = cur_PC10;
}

static void ProcessSpawnTick(GameSession *session)
{
    /* The tick pipeline is: delay -> spawn -> gravity/lock. */
    if (session->spawn_delay_active) {
        session->spawn_counter++;
        if (session->spawn_counter >= session->spawn_ticks_per_spawn) {
            session->spawn_delay_active = 0;
            session->spawn_counter = 0;
            session->spawnNew = 1;
        }
        return;
    }

    if (session->spawnNew) {
        session->tetromino = SpawnQueuedTetromino(&session->originX, &session->originY);
        session->rotation = 0;

        if (!CanPlaceTetromino(session->tetromino,
                               session->originX,
                               session->originY,
                               session->rotation)) {
            HandleGameOver(session);
        } else {
            session->spawnNew = 0;
        }
        return;
    }

    ClearTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    if (CanMoveDown(session->tetromino, session->originX, session->originY, session->rotation)) {
        session->originY += STONE_BLOCK;
        DrawTetromino(session->tetromino, session->originX, session->originY, session->rotation);
        return;
    }

    DrawTetromino(session->tetromino, session->originX, session->originY, session->rotation);
    LockActiveTetromino(session);
}

void GameSession_WaitForStart(void)
{
    uint8_t cur_PA0;

    /* SW1 uses an internal pull-up and active-low input. */
    GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);
    PA->DBEN |= BIT0;
    PA->PUSEL |= BIT0;
    PA0 = 0;
    g_buttons.prev_PA0 = PA0;
    TimerDelay_Start(0);

    while (1) {
        if (Timer3_flag) {
            Timer3_flag = 0;
        }
        if (!TimerDelay_Done()) {
            continue;
        }

        cur_PA0 = PA0;
        if (g_buttons.prev_PA0 && !cur_PA0) {
            TimerDelay_Start(2);
            do {
                if (Timer3_flag) {
                    Timer3_flag = 0;
                }
            } while (!PA0);
            g_buttons.prev_PA0 = 1;
            return;
        }
        g_buttons.prev_PA0 = cur_PA0;
    }
}

void GameSession_ConfigureControls(void)
{
    /* Configure gameplay buttons as debounced active-low inputs. */
    GPIO_SetMode(PG, BIT2 | BIT3 | BIT4, GPIO_MODE_INPUT);
    GPIO_SetMode(PC, BIT9 | BIT10, GPIO_MODE_INPUT);
    GPIO_SetMode(PA, BIT0 | BIT1, GPIO_MODE_INPUT);

    PG->DBEN |= BIT2 | BIT3 | BIT4;
    PC->DBEN |= BIT9 | BIT10;
    PA->DBEN |= BIT0 | BIT1;

    PG->PUSEL |= BIT2 | BIT4;
    PC->PUSEL |= BIT9 | BIT10;
    PA->PUSEL |= BIT0 | BIT1;

    PG2 = PG3 = PG4 = PC9 = PC10 = PA0 = PA1 = 0;

    g_buttons.prev_PA0 = 1;
    g_buttons.prev_PA1 = 1;
    g_buttons.prev_PG2 = 1;
    g_buttons.prev_PG4 = 1;
    g_buttons.prev_PC9 = 1;
    g_buttons.prev_PC10 = 1;
}

void GameSession_Begin(GameSession *session)
{
    /* Start from a clean board with a seeded next-piece queue. */
    LCD_BlankArea(0, 0, LCD_W, LCD_H, UI_BG_COLOR);
    DrawGameplayScreen();

    ResetGameStats(session);
    InitializeTetrominoQueue();
    ClearPlayfield();
    ScheduleSpawnDelay(session);
}

void GameSession_Run(GameSession *session)
{
    uint8_t cur_PA0;
    uint8_t cur_PA1;
    uint8_t cur_PG2;
    uint8_t cur_PG4;
    uint8_t cur_PC9;
    uint8_t cur_PC10;
    uint8_t ticks_per_drop;

    while (1) {
        /* Consume the shared timer flag before input and gravity checks. */
        if (Timer3_flag) {
            Timer3_flag = 0;
        }

        if (TimerDelay_Done()) {
            cur_PA0 = PA0;
            cur_PA1 = PA1;
            cur_PG2 = PG2;
            cur_PG4 = PG4;
            cur_PC9 = PC9;
            cur_PC10 = PC10;

            if (session->game_over) {
                HandleGameOverInput(session, cur_PA0, cur_PA1);
                continue;
            }

            HandleNormalInput(session, cur_PA0, cur_PA1, cur_PG2, cur_PG4, cur_PC9, cur_PC10);
        }

        if (Timer3_flag && !session->isPaused && !session->game_over) {
            Timer3_flag = 0;
            ticks_per_drop = GetTicksForLevel();
            session->gravity_counter++;

            if (session->gravity_counter >= ticks_per_drop) {
                /* Apply gravity only when the level-adjusted interval expires. */
                session->gravity_counter = 0;
                ProcessSpawnTick(session);
            }
        }
    }
}
