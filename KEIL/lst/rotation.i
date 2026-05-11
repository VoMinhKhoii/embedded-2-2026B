# 1 "rotation.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 400 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "rotation.c" 2
# 1 ".\\rotation.h" 1



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
# 5 ".\\rotation.h" 2

void RotateTetrominoClockwise(TetrominoType t,
                             uint16_t *x, uint16_t *y, uint8_t *rot);
# 2 "rotation.c" 2
# 1 ".\\playfield.h" 1
# 11 ".\\playfield.h"
extern uint8_t playfield[14][30];
static uint16_t locked_count = 0;


typedef struct {
    uint8_t col; // column index (0 to 14 -1)
    uint8_t row; // row index (0 to 30 -1)
    TetrominoType type; // block type/color
} LockedBlock;
static LockedBlock locked_blocks[400];
static int locked_block_count = 0;
void ClearPlayfield(void);
void RedrawPlayfield(void);
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
# 3 "rotation.c" 2
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
# 4 "rotation.c" 2
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
# 5 "rotation.c" 2
// Declare Timer3_cnt from EBI_LCD_Module.c (increments every 100ms)
extern volatile uint8_t Timer3_cnt;

// Debounce rotation: minimum 1 tick (100ms) between rotations
static const uint8_t rotation_debounce_ticks = 1; // 100ms
static uint8_t last_rotation_tick = 0;

void RotateTetrominoClockwise(TetrominoType t, uint16_t *x, uint16_t *y, uint8_t *rot)
{
    uint8_t new_rot;
    uint16_t tryX, tryY;


    if ((uint8_t)(Timer3_cnt - last_rotation_tick) < rotation_debounce_ticks) {
        return;
    }

    new_rot = (uint8_t)((*rot + 1) & 3);
    tryY = *y;


    if (CanRotate(t, *x, tryY, new_rot)) {

        ClearTetromino(t, *x, tryY, *rot);
        *rot = new_rot;
    }
    else {

        tryX = *x - 10;
        if (CanRotate(t, tryX, tryY, new_rot)) {
            ClearTetromino(t, *x, tryY, *rot);
            *x = tryX;
            *rot = new_rot;
        }
        else {

            tryX = *x + 10;
            if (CanRotate(t, tryX, tryY, new_rot)) {
                ClearTetromino(t, *x, tryY, *rot);
                *x = tryX;
                *rot = new_rot;
            }
            else {

                return;
            }
        }
    }


    DrawTetromino(t, *x, tryY, *rot);


    last_rotation_tick = Timer3_cnt;
}
