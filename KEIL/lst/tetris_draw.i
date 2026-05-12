# 1 "tetris_draw.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 400 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "tetris_draw.c" 2
// tetris_draw.c
# 1 ".\\tetris_draw.h" 1
# 13 ".\\tetris_draw.h"
// NEXT box geometry




// height of the 8?16 ?NEXT? label

# 1 "C:\\Users\\Tam Tran\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 1 3
# 56 "C:\\Users\\Tam Tran\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 3
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;





typedef signed char int_least8_t;
typedef signed short int int_least16_t;
typedef signed int int_least32_t;
typedef signed long long int int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long int uint_least64_t;




typedef signed int int_fast8_t;
typedef signed int int_fast16_t;
typedef signed int int_fast32_t;
typedef signed long long int int_fast64_t;


typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long int uint_fast64_t;






typedef signed int intptr_t;
typedef unsigned int uintptr_t;



typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
# 21 ".\\tetris_draw.h" 2
# 1 ".\\tetromino_spawn.h" 1





typedef enum {
    TETROMINO_I,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_T,
    TETROMINO_Z,
    TETROMINO_COUNT
} TetrominoType;

void InitializeTetrominoQueue(void);

TetrominoType SpawnQueuedTetromino(uint16_t *originX, uint16_t *originY);
# 22 ".\\tetris_draw.h" 2
static const int8_t tetro[7][4][4][2] = {
    // I
    {{{0,1},{1,1},{2,1},{3,1}},
     {{2,0},{2,1},{2,2},{2,3}},
     {{0,2},{1,2},{2,2},{3,2}},
     {{1,0},{1,1},{1,2},{1,3}}},
    // J
    {{{0,0},{0,1},{1,1},{2,1}},
     {{1,0},{2,0},{1,1},{1,2}},
     {{0,1},{1,1},{2,1},{2,2}},
     {{1,0},{1,1},{0,2},{1,2}}},
    // L
    {{{2,0},{0,1},{1,1},{2,1}},
     {{0,0},{0,1},{0,2},{1,2}},
     {{0,1},{1,1},{2,1},{0,2}},
     {{0,0},{1,0},{1,1},{1,2}}},
    // O
    {{{0,0},{1,0},{0,1},{1,1}},
     {{0,0},{1,0},{0,1},{1,1}},
     {{0,0},{1,0},{0,1},{1,1}},
     {{0,0},{1,0},{0,1},{1,1}}},
    // S
    {{{1,0},{2,0},{0,1},{1,1}},
     {{0,0},{0,1},{1,1},{1,2}},
     {{1,0},{2,0},{0,1},{1,1}},
     {{0,0},{0,1},{1,1},{1,2}}},
    // T
    {{{0,0},{1,0},{2,0},{1,1}},
     {{1,0},{1,1},{2,1},{1,2}},
     {{1,0},{0,1},{1,1},{2,1}},
     {{1,0},{0,1},{1,1},{1,2}}},
    // Z
    {{{0,0},{1,0},{1,1},{2,1}},
     {{1,0},{0,1},{1,1},{0,2}},
     {{0,0},{1,0},{1,1},{2,1}},
     {{1,0},{0,1},{1,1},{0,2}}}
};

void DrawTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot);
void ClearTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot);
void DrawNextTetromino(TetrominoType t);
void DrawBlock(uint16_t x, uint16_t y, TetrominoType type);
# 3 "tetris_draw.c" 2
# 1 "../../task2-complete\\EBI_LCD_Module.h" 1
# 29 "../../task2-complete\\EBI_LCD_Module.h"
extern uint8_t Font8x16[];
extern uint16_t Font16x32[];
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t timer_running;


void ILI9341_Initial(void);
void Timer3_Init(void);
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_DATA(uint16_t dat);
uint16_t LCD_RD_DATA(void);
void LCD_SetWindow(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);
void LCD_PutString(uint16_t x, uint16_t y, uint8_t *s, uint32_t fColor, uint32_t bColor);
void LCD_PutChar16x32(uint16_t x, uint16_t y, uint16_t c, uint32_t fColor, uint32_t bColor);
void LCD_BlankArea(uint16_t X, uint16_t Y, uint16_t W, uint16_t H, uint16_t color);
uint16_t Get_TP_X(void);
uint16_t Get_TP_Y(void);
void TimerDelay_Start(uint8_t ticks);
uint8_t TimerDelay_Done(void);
# 4 "tetris_draw.c" 2
# 1 ".\\game_screens.h" 1
# 15 ".\\game_screens.h"
extern uint16_t currentScore;
extern uint8_t level;
extern uint8_t minutes;
extern uint8_t seconds;
extern char acString[32];
extern uint16_t currentHighScore;
void DrawScreenBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DrawGameplayScreen(void);
void DrawStartScreen(void);
void DrawGameOverScreen(void);
void DrawLeaderboardScreen(void);

void RecordLeaderboardEntry(uint16_t score,
                            uint8_t level,
                            uint8_t minutes,
                            uint8_t seconds);
# 5 "tetris_draw.c" 2
# 1 ".\\playfield.h" 1
# 11 ".\\playfield.h"
extern uint8_t playfield[14][30];
static uint16_t locked_count = 0;


typedef struct {
    uint8_t col;
    uint8_t row;
    TetrominoType type;
} LockedBlock;
static LockedBlock locked_blocks[400];
static int locked_block_count = 0;
void ClearPlayfield(void);
void RedrawPlayfield(void);
void DrawPlayfieldBackground(void);
void DrawPlayfieldCellBackground(uint16_t x, uint16_t y);
uint8_t CanPlaceTetromino(TetrominoType t,
                          uint16_t x, uint16_t y, uint8_t rot);
uint8_t CanMoveDown(TetrominoType t,
                    uint16_t x, uint16_t y, uint8_t rot);
uint8_t CanMoveLeft(TetrominoType t,
                    uint16_t x, uint16_t y, uint8_t rot);
uint8_t CanMoveRight(TetrominoType t,
                     uint16_t x, uint16_t y, uint8_t rot);
uint8_t CanRotate(TetrominoType t,
                  uint16_t x, uint16_t y, uint8_t rot);

int ClearFullLines(void);

void LockTetromino(TetrominoType t,
                   uint16_t x, uint16_t y, uint8_t rot);
# 6 "tetris_draw.c" 2



// Colors in the same order as TetrominoType enum: I,J,L,O,S,T,Z
static const uint16_t colorOf[7] = {
    0x07FF, 0x001F, 0xFD20,
    0xFFE0, 0x07E0, 0x780F,
    0xF800
};

// dx,dy offsets for each piece (I=0?Z=6), each rotation 0?3, each of 4 blocks

void DrawTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int i;
    int8_t dx, dy;
    uint16_t px, py;
    uint16_t colColor = colorOf[t];

    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        px = x + dx * 10;
        py = y + dy * 10;
        LCD_BlankArea(px, py, 10, 10, colColor);
    }
}

void ClearTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int i;
    int8_t dx, dy;
    uint16_t px, py;

    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        px = x + dx * 10;
        py = y + dy * 10;
        DrawPlayfieldCellBackground(px, py);
    }
}

void DrawNextTetromino(TetrominoType t)
{
    // carve out exactly the interior under ?NEXT? but inside the 10px stone border
    const int16_t innerX = 150 + 10;
    const int16_t innerY = 0 + 16 + 5;
    const int16_t innerW = 90 - 2*10;
    const int16_t innerH = 60 - 16 - 2*10;

    // 1) clear only that region
    LCD_BlankArea(
        innerX,
        innerY,
        innerW,
        innerH,
        0x963F
    );

    // 2) compute the spawn-rotation (rot=0) bounds of this tetromino
    {
        int8_t min_dx = tetro[t][0][0][0];
        int8_t max_dx = min_dx;
        int8_t min_dy = tetro[t][0][0][1];
        int8_t max_dy = min_dy;
        int i;
        for (i = 1; i < 4; i++) {
            int8_t dx = tetro[t][0][i][0];
            int8_t dy = tetro[t][0][i][1];
            if (dx < min_dx) min_dx = dx;
            if (dx > max_dx) max_dx = dx;
            if (dy < min_dy) min_dy = dy;
            if (dy > max_dy) max_dy = dy;
        }

        // convert to pixels
        const int16_t shapeW = (max_dx - min_dx + 1) * 10;
        const int16_t shapeH = (max_dy - min_dy + 1) * 10;

        // center within the cleared area, then offset by -min_dx/-min_dy
        const int16_t drawX = innerX
                            + (innerW - shapeW) / 2
                            - min_dx * 10;
        const int16_t drawY = innerY
                            + (innerH - shapeH) / 2
                            - min_dy * 10;

        // 3) draw the next piece (rotation=0)
        DrawTetromino(t, drawX, drawY, 0);
    }


    LCD_PutString(179, 10, (uint8_t *)"NEXT", 0x0000, 0x963F);
}
void DrawBlock(uint16_t x, uint16_t y, TetrominoType type)
{
    // Draw one 10x10 square block with color matching type
    static const uint16_t colors[] = {
        0x07FF, 0x001F, 0xFD20, 0xFFE0,
        0x07E0, 0x780F, 0xF800
    };

    uint16_t color = (type >= TETROMINO_COUNT) ? 0xFFFF : colors[type];

    LCD_BlankArea(x, y, 10, 10, color);
}
