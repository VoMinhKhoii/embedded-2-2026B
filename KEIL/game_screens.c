#include "game_screens.h"
#include "EBI_LCD_Module.h"
#include "start_logo_rgb565.h"
#include "start_skyline_rgb565.h"
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


void RecordLeaderboardEntry(uint16_t score,
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


void DrawScreenBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t i, j, color;
    for (i = 0; i + STONE_BLOCK <= w; i += STONE_BLOCK) {
        //color = ((i / STONE_BLOCK) & 1) ? C_STONE_B : C_STONE_A;
        LCD_BlankArea(x+i,y,STONE_BLOCK,STONE_BLOCK,STONE_COLOR);
    }
    for (i = 0; i + STONE_BLOCK <= w; i += STONE_BLOCK) {
        // color = ((i / STONE_BLOCK) & 1) ? C_STONE_A : C_STONE_B;
        LCD_BlankArea(x+i,y+h- STONE_BLOCK,STONE_BLOCK,STONE_BLOCK,STONE_COLOR);
    }
    for (j = 0; j + STONE_BLOCK <= h; j += STONE_BLOCK) {
        // color = ((j / STONE_BLOCK) & 1) ? C_STONE_B : C_STONE_A;	
        LCD_BlankArea(x,y+j,STONE_BLOCK,STONE_BLOCK,STONE_COLOR);
    }
    for (j = 0; j + STONE_BLOCK <= h; j += STONE_BLOCK) {
        // color = ((j / STONE_BLOCK) & 1) ? C_STONE_A : C_STONE_B;
        LCD_BlankArea(x+w- STONE_BLOCK,y+j,STONE_BLOCK,STONE_BLOCK,STONE_COLOR);
    }
}


void DrawGameplayScreen(void)
{
	char hiString[10];
    LCD_BlankArea(0,0,240,320,UI_BG_COLOR);
    LCD_BlankArea(160,10,70,300,RIGHT_PANEL_BG_COLOR);
    DrawScreenBorder(0, 0, 240, 320);
    DrawScreenBorder(0, 0, 160, 320);
    DrawScreenBorder(150, 0,  90, 60);
    DrawScreenBorder(150, 0,  90, 225);
    LCD_PutString(179, 10, (uint8_t*)"NEXT",   0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,70, (uint8_t*)"TOP",    0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,105, (uint8_t*)"SCORE",  0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,137, (uint8_t*)"TIME",   0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,175, (uint8_t*)"LEVEL",  0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,235, (uint8_t*)"PAUSE",  0x01AF, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,250, (uint8_t*)"SW1",    0x0000, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,275, (uint8_t*)"RESTART",0x01AF, RIGHT_PANEL_BG_COLOR);
    LCD_PutString(167,290, (uint8_t*)"SW2",    0x0000, RIGHT_PANEL_BG_COLOR);
    /* Draw the current session high score under its label. */
    sprintf(hiString, "%u", currentHighScore);
    LCD_PutString(167, 85, (uint8_t*)hiString, 0x01AF, RIGHT_PANEL_BG_COLOR);
}

static void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    LCD_BlankArea(x, y, w, h, color);
}

static uint16_t MapLogoColor(uint16_t raw_color)
{
    switch (raw_color) {
    case 151:
        return 0xAFB7;
    case 214:
        return 0x571D;
    case 100:
        return 0x0374;
    case 41:
        return 0x04B8;
    case 6:
        return 0x05DB;
    case 0:
    default:
        return 0x4A89;
    }
}

static void DrawTetrisLogo(uint16_t x, uint16_t y)
{
    uint16_t pixel_w;
    uint16_t row;
    uint16_t col;

    pixel_w = 212 / START_LOGO_RGB565_W;
    if (pixel_w == 0) {
        pixel_w = 1;
    }

    for (row = 0; row < START_LOGO_RGB565_H; row++) {
        for (col = 0; col < START_LOGO_RGB565_W; col++) {
            FillRect(x + col * pixel_w,
                     y + row,
                     pixel_w,
                     1,
                     MapLogoColor(start_logo_rgb565[row * START_LOGO_RGB565_W + col]));
        }
    }
}

static void DrawSkylinePanel(uint16_t x,
                             uint16_t y,
                             uint16_t w,
                             uint16_t h,
                             uint16_t light,
                             uint16_t mid,
                             uint16_t dark,
                             uint16_t ink)
{
    uint16_t pixel_w;
    uint16_t pixel_h;
    uint16_t row;
    uint16_t col;
    (void)light;
    (void)mid;
    (void)dark;
    (void)ink;

    pixel_w = w / START_SKYLINE_RGB565_W;
    pixel_h = h / START_SKYLINE_RGB565_H;

    if (pixel_w == 0 || pixel_h == 0) {
        FillRect(x, y, w, h, UI_BG_COLOR);
        return;
    }

    for (row = 0; row < START_SKYLINE_RGB565_H; row++) {
        for (col = 0; col < START_SKYLINE_RGB565_W; col++) {
            FillRect(x + col * pixel_w,
                     y + row * pixel_h,
                     pixel_w,
                     pixel_h,
                     start_skyline_rgb565[row * START_SKYLINE_RGB565_W + col]);
        }
    }
}

static void DrawMenuLane(uint16_t x, uint16_t y, uint16_t w, uint16_t dark, uint16_t light)
{
    (void)dark;
    FillRect(x, y, w, 18, light);
}


void DrawStartScreen(void)
{
    static const uint16_t bg_light = 0xAFB7;
    static const uint16_t bg_mid   = 0x8E0E;
    static const uint16_t bg_dark  = 0x334A;
    static const uint16_t ink_dark = 0x08C3;
    static const uint16_t ink_soft = 0x334A;
    static const uint16_t ink_pale = 0xAFB7;

    FillRect(0, 0, LCD_W, LCD_H, bg_light);
    FillRect(6, 6, 228, 308, ink_dark);
    FillRect(10, 10, 220, 300, ink_pale);

    /* Title band and logo. */
    FillRect(14, 14, 212, 52, ink_soft);
    DrawTetrisLogo(14, 16);

    /* Main art panel. */
    DrawSkylinePanel(14, 70, 212, 102, bg_mid, ink_soft, bg_dark, ink_dark);

    /* Menu lanes inspired by the original 1P/2P strip. */
    DrawMenuLane(14, 180, 94, ink_dark, ink_pale);
    DrawMenuLane(132, 180, 94, ink_dark, ink_pale);
    LCD_PutString(22, 184, (uint8_t *)"Start Game", ink_dark, ink_pale);
    LCD_PutString(140, 184, (uint8_t *)"Auto Play", ink_dark, ink_pale);
    LCD_PutString(40, 202, (uint8_t *)"SW1", ink_dark, ink_pale);
    LCD_PutString(168, 202, (uint8_t *)"SW2", ink_dark, ink_pale);

    /* Team credits. */
    LCD_PutString(96, 228, (uint8_t *)"Team 8", ink_dark, ink_pale);
    LCD_PutString(18, 248, (uint8_t *)"Tran Tu Tam", ink_dark, ink_pale);
    LCD_PutString(160, 248, (uint8_t *)"S3999159", ink_dark, ink_pale);
    LCD_PutString(18, 266, (uint8_t *)"Vo Minh Khoi", ink_dark, ink_pale);
    LCD_PutString(160, 266, (uint8_t *)"S3991730", ink_dark, ink_pale);
    LCD_PutString(18, 284, (uint8_t *)"Doan Phuc Khoa", ink_dark, ink_pale);
    LCD_PutString(160, 284, (uint8_t *)"S3927381", ink_dark, ink_pale);
}


void DrawGameOverScreen(void)
{
    LCD_BlankArea(0, 0, LCD_W, LCD_H, UI_BG_COLOR);
    LCD_PutString(80,10,(uint8_t*)"GAME OVER", C_YELLOW, UI_BG_COLOR);
    LCD_PutString(70,250,(uint8_t*)"Press SW2 to restart", C_YELLOW, UI_BG_COLOR);
    LCD_PutString(35,300,(uint8_t*)"Press SW1 for LeaderBoard", C_YELLOW, UI_BG_COLOR);
    /* Show the final score, level, and elapsed time. */
    char timeString[6];
    sprintf(timeString, "%02d:%02d", minutes, seconds);
    LCD_PutString(105, 100, (uint8_t*)"TIME", C_WHITE, UI_BG_COLOR);
    LCD_PutString(105, 130, (uint8_t*)timeString, C_WHITE, UI_BG_COLOR);
    LCD_PutString(25, 100, (uint8_t*)"SCORE", C_WHITE, UI_BG_COLOR);
    sprintf(acString, "%d", currentScore);
    LCD_PutString(25, 130, (uint8_t*)acString, C_WHITE, UI_BG_COLOR);
    LCD_PutString(185, 100, (uint8_t*)"LEVEL", C_WHITE, UI_BG_COLOR);
    sprintf(acString, "%d", level);
    LCD_PutString(185, 130, (uint8_t*)acString, C_WHITE, UI_BG_COLOR);
}

/* Draw the top leaderboard entries and restart prompt. */
void DrawLeaderboardScreen(void)
{
    char entryString[32];
    uint8_t i;
    uint8_t toShow;
    uint16_t y;

    /* Clear the screen and draw the table header. */
    LCD_BlankArea(0, 0, LCD_W, LCD_H, UI_BG_COLOR);
    LCD_PutString(70,  10, (uint8_t*)"LEADERBOARD",      C_YELLOW, UI_BG_COLOR);
    LCD_PutString( 10,  30, (uint8_t*)"Rank",            C_WHITE, UI_BG_COLOR);
    LCD_PutString(55, 30, (uint8_t*)"Score",           C_WHITE, UI_BG_COLOR);
    LCD_PutString(120,  30, (uint8_t*)"Level",           C_WHITE, UI_BG_COLOR);
    LCD_PutString(180,  30, (uint8_t*)"Time",            C_WHITE, UI_BG_COLOR);

    if (leaderboard_count < 10) {
        toShow = leaderboard_count;
    } else {
        toShow = 10;
    }

    for (i = 0; i < toShow; i++)
    {
        y = 50 + (uint16_t)i * 20;
        sprintf(entryString, "%2u", (uint16_t)(i + 1));
        LCD_PutString( 10, y, (uint8_t*)entryString, C_WHITE, UI_BG_COLOR);
        sprintf(entryString, "%5u", leaderboard[i].score);
        LCD_PutString( 55, y, (uint8_t*)entryString, C_WHITE, UI_BG_COLOR);
        sprintf(entryString, "L%1u", leaderboard[i].level);
        LCD_PutString(120, y, (uint8_t*)entryString, C_WHITE, UI_BG_COLOR);
        sprintf(entryString, "%02u:%02u",
                leaderboard[i].minutes,
                leaderboard[i].seconds);
        LCD_PutString(180, y, (uint8_t*)entryString, C_WHITE, UI_BG_COLOR);
    }

    /* Keep restart instructions visible at the bottom of the screen. */
    LCD_PutString( 60, 300, (uint8_t*)"SW2 TO RESTART",    C_WHITE, UI_BG_COLOR);
}
