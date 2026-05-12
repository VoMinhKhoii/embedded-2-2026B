#include "game_draw.h"
#include "EBI_LCD_Module.h"
#include <stdio.h>
uint16_t currentScore = 0;
uint8_t  level        = 1;
uint16_t currentHighScore = 0;
typedef struct {
    uint16_t score;
    uint8_t  level;
    uint8_t  minutes;
    uint8_t  seconds;
} LeaderEntry;
static LeaderEntry leaderboard[100];
static uint8_t leaderboard_count = 0;
char acString[32];  


void AddToLeaderboard(uint16_t score,
                      uint8_t  level,
                      uint8_t  minutes,
                      uint8_t  seconds)
{
    LeaderEntry newEntry;
    uint8_t     pos;
    uint8_t     i;

    /* Create the candidate entry. */
    newEntry.score   = score;
    newEntry.level   = level;
    newEntry.minutes = minutes;
    newEntry.seconds = seconds;

    /* Find the insertion point in descending score order. */
    pos = leaderboard_count;
    for (i = 0; i < leaderboard_count; i++) {
        if (score > leaderboard[i].score) {
            pos = i;
            break;
        }
    }

    /* Shift existing entries to make room if this score qualifies. */
    if (leaderboard_count < 100) {
        for (i = leaderboard_count; i > pos; i--) {
            leaderboard[i] = leaderboard[i - 1];
        }
        leaderboard[pos] = newEntry;
        leaderboard_count++;
    }
    else {
        if (pos < 100) {
            for (i = 99; i > pos; i--) {
                leaderboard[i] = leaderboard[i - 1];
            }
            leaderboard[pos] = newEntry;
        }
        /* Ignore scores that fall outside the top 100. */
    }

    /* Track the best score reached during this power cycle. */
    if (score > currentHighScore) {
        currentHighScore = score;
    }
}


void DrawStoneBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t i, j, color;
    for (i = 0; i + STONE_BLOCK <= w; i += STONE_BLOCK) {
        color = ((i / STONE_BLOCK) & 1) ? C_STONE_B : C_STONE_A;
        LCD_BlankArea(x+i,y,STONE_BLOCK,STONE_BLOCK,color);
    }
    for (i = 0; i + STONE_BLOCK <= w; i += STONE_BLOCK) {
        color = ((i / STONE_BLOCK) & 1) ? C_STONE_A : C_STONE_B;
        LCD_BlankArea(x+i,y+h- STONE_BLOCK,STONE_BLOCK,STONE_BLOCK,color);
    }
    for (j = 0; j + STONE_BLOCK <= h; j += STONE_BLOCK) {
        color = ((j / STONE_BLOCK) & 1) ? C_STONE_B : C_STONE_A;	
        LCD_BlankArea(x,y+j,STONE_BLOCK,STONE_BLOCK,color);
    }
    for (j = 0; j + STONE_BLOCK <= h; j += STONE_BLOCK) {
        color = ((j / STONE_BLOCK) & 1) ? C_STONE_A : C_STONE_B;
        LCD_BlankArea(x+w- STONE_BLOCK,y+j,STONE_BLOCK,STONE_BLOCK,color);
    }
}


void DisplayGameField(void)
{
		char hiString[10];
    LCD_BlankArea(0,0,240,320,C_BLACK);
    DrawStoneBorder(0, 0, 240, 320);
    DrawStoneBorder(0, 0, 160, 320);
    DrawStoneBorder(150, 0,  90, 100);
    DrawStoneBorder(150, 0,  90, 245);
    LCD_PutString(179, 10, (uint8_t*)"NEXT",   C_WHITE, C_BLACK);
    LCD_PutString(167,100, (uint8_t*)"HISCORE",C_WHITE, C_BLACK);
    LCD_PutString(176,135, (uint8_t*)"SCORE",  C_WHITE, C_BLACK);
    LCD_PutString(178,165, (uint8_t*)"TIME",   C_WHITE, C_BLACK);
    LCD_PutString(176,195, (uint8_t*)"LEVEL",  C_WHITE, C_BLACK);
    LCD_PutString(173,245, (uint8_t*)"SW1 TO", C_WHITE, C_BLACK);
    LCD_PutString(176,260, (uint8_t*)"PAUSE",  C_WHITE, C_BLACK);
    LCD_PutString(173,275, (uint8_t*)"SW2 TO", C_WHITE, C_BLACK);
    LCD_PutString(171,290, (uint8_t*)"RESTART",C_WHITE, C_BLACK);
    /* Draw the current session high score under its label. */
    sprintf(hiString, "%u", currentHighScore);
    LCD_PutString(176, 115, (uint8_t*)hiString, C_WHITE, C_BLACK);
}


void StartGameField(void)
{
		LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
		LCD_PutString(85,10,(uint8_t*)"TETRIS ", C_YELLOW, C_BLACK);
		LCD_PutString(0,160,(uint8_t*)"\tPress SW1 to start",       C_YELLOW, C_BLACK);
		LCD_PutString(0,234,(uint8_t*)"Nguyen Tri Khai - s3921136",       C_BLUE2, C_BLACK);
		LCD_PutString(0,250,(uint8_t*)"Le Nguyen Hao - s3963228",       C_BLUE2, C_BLACK);
}


void GameOverField(void)
{
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    LCD_PutString(80,10,(uint8_t*)"GAME OVER", C_YELLOW, C_BLACK);
    LCD_PutString(70,250,(uint8_t*)"Press SW2 to restart", C_YELLOW, C_BLACK);
    LCD_PutString(35,300,(uint8_t*)"Press SW1 for LeaderBoard", C_YELLOW, C_BLACK);
    /* Show the final score, level, and elapsed time. */
    char timeString[6];
    sprintf(timeString, "%02d:%02d", minutes, seconds);
    LCD_PutString(105, 100, (uint8_t*)"TIME", C_WHITE, C_BLACK);
    LCD_PutString(105, 130, (uint8_t*)timeString, C_WHITE, C_BLACK);
    LCD_PutString(25, 100, (uint8_t*)"SCORE", C_WHITE, C_BLACK);
    sprintf(acString, "%d", currentScore);
    LCD_PutString(25, 130, (uint8_t*)acString, C_WHITE, C_BLACK);
    LCD_PutString(185, 100, (uint8_t*)"LEVEL", C_WHITE, C_BLACK);
    sprintf(acString, "%d", level);
    LCD_PutString(185, 130, (uint8_t*)acString, C_WHITE, C_BLACK);
}

/* Draw the top leaderboard entries and restart prompt. */
void LeaderBoard(void)
{
    char entryString[32];
    uint8_t i;
    uint8_t toShow;
    uint16_t y;

    /* Clear the screen and draw the table header. */
    LCD_BlankArea(0, 0, LCD_W, LCD_H, C_BLACK);
    LCD_PutString(70,  10, (uint8_t*)"LEADERBOARD",      C_YELLOW, C_BLACK);
    LCD_PutString( 10,  30, (uint8_t*)"Rank",            C_WHITE, C_BLACK);
    LCD_PutString(55, 30, (uint8_t*)"Score",           C_WHITE, C_BLACK);
    LCD_PutString(120,  30, (uint8_t*)"Level",           C_WHITE, C_BLACK);
    LCD_PutString(180,  30, (uint8_t*)"Time",            C_WHITE, C_BLACK);

    if (leaderboard_count < 10) {
        toShow = leaderboard_count;
    } else {
        toShow = 10;
    }

    for (i = 0; i < toShow; i++)
    {
        y = 50 + (uint16_t)i * 20;
        sprintf(entryString, "%2u", (uint16_t)(i + 1));
        LCD_PutString( 10, y, (uint8_t*)entryString, C_WHITE, C_BLACK);
        sprintf(entryString, "%5u", leaderboard[i].score);
        LCD_PutString( 55, y, (uint8_t*)entryString, C_WHITE, C_BLACK);
        sprintf(entryString, "L%1u", leaderboard[i].level);
        LCD_PutString(120, y, (uint8_t*)entryString, C_WHITE, C_BLACK);
        sprintf(entryString, "%02u:%02u",
                leaderboard[i].minutes,
                leaderboard[i].seconds);
        LCD_PutString(180, y, (uint8_t*)entryString, C_WHITE, C_BLACK);
    }

    /* Keep restart instructions visible at the bottom of the screen. */
    LCD_PutString( 60, 300, (uint8_t*)"SW2 TO RESTART",    C_WHITE, C_BLACK);
}
