# 1 "spawn_block.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 400 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "spawn_block.c" 2
// spawn_block.c

# 1 ".\\spawn_block.h" 1



# 1 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 1 3
# 56 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 3
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
# 5 ".\\spawn_block.h" 2

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

void InitSpawn(void);
// Function to spawn a new tetromino
TetrominoType SpawnTetromino(uint16_t *originX, uint16_t *originY);
# 4 "spawn_block.c" 2
# 1 ".\\tetris_draw.h" 1
# 13 ".\\tetris_draw.h"
// NEXT box geometry




// height of the 8×16 “NEXT” label



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
# 5 "spawn_block.c" 2
# 1 ".\\game_draw.h" 1
# 11 ".\\game_draw.h"
extern uint16_t currentScore; // Declare currentScore as extern

extern uint8_t level; // Declare level as extern
extern uint8_t minutes; // Declare as extern
extern uint8_t seconds; // Declare as extern
extern char acString[32]; // This should be large enough to hold the score, level, and time

extern uint16_t currentHighScore; // NEW: highest ever this session
void DrawStoneBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DisplayGameField(void);
void StartGameField(void);
void GameOverField(void);
void LeaderBoard(void);

void AddToLeaderboard(uint16_t score,
                      uint8_t level,
                      uint8_t minutes,
                      uint8_t seconds);
# 6 "spawn_block.c" 2
# 1 "../../task2-complete\\EBI_LCD_Module.h" 1
# 28 "../../task2-complete\\EBI_LCD_Module.h"
// Characters
extern uint8_t Font8x16[];
extern uint16_t Font16x32[];
extern uint8_t minutes; // minutes counter
extern uint8_t seconds; // seconds counter
extern uint8_t timer_running; // 0 = paused (splash/Game Over), 1 = play clock

// Sub-functions
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
# 7 "spawn_block.c" 2



extern volatile uint8_t Timer3_cnt;
// simple linear congruential generator (LCG) for pseudo-random numbers
static uint32_t lcg_seed; // no fixed initializer any more

// what piece is queued up next:
static TetrominoType _next_piece;

// helper RNG
static uint32_t get_random(uint32_t modulus)
{
    lcg_seed = (1103515245 * lcg_seed + 12345) & 0x7FFFFFFF;
    return lcg_seed % modulus;
}

// must be called once at startup to seed and draw the very first preview
void InitSpawn(void)
{
    // seed with Timer3_cnt so the user’s press timing randomizes it
    lcg_seed = (uint32_t)Timer3_cnt;

    // pick initial “next” piece
    _next_piece = (TetrominoType)get_random(7);
    DrawNextTetromino(_next_piece);
}

// spawn the queued piece, draw it at the top-center of the playfield,
// then immediately pick & preview the *new* next piece
TetrominoType SpawnTetromino(uint16_t *originX, uint16_t *originY)
{
    *originX = 5 * 10;
    *originY = 10;

    TetrominoType curr = _next_piece;
    DrawTetromino(curr, *originX, *originY, 0);

    _next_piece = (TetrominoType)get_random(7);
    DrawNextTetromino(_next_piece);

    return curr;
}
