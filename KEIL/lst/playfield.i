# 1 "playfield.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 400 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "playfield.c" 2
# 1 ".\\playfield.h" 1






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
# 8 ".\\playfield.h" 2
# 1 ".\\spawn_block.h" 1





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
# 9 ".\\playfield.h" 2


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
# 2 "playfield.c" 2
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
# 3 "playfield.c" 2
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
# 4 "playfield.c" 2
uint8_t playfield[14][30];

void ClearPlayfield(void)
{
    int c, r;

    for (c = 0; c < 14; c++)
        for (r = 0; r < 30; r++)
            playfield[c][r] = 0;

    locked_block_count = 0; // reset the real block-count


    LCD_BlankArea(
      10,
      10,
      14 * 10,
      30 * 10,
      0x0000
    );
}
void RedrawPlayfield(void)
{
    int i;


    LCD_BlankArea(
      10,
      10,
      14 * 10,
      30 * 10,
      0x0000
    );


    for (i = 0; i < locked_block_count; i++)
    {
        LockedBlock *lb = &locked_blocks[i];
        DrawBlock(
          10 + lb->col * 10,
          10 + lb->row * 10,
          lb->type
        );
    }
}
uint8_t CanPlaceTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int16_t col = ((int16_t)x - 10) / 10;
    int16_t row = ((int16_t)y - 10 ) / 10;
    int i;
    int8_t dx, dy;
    int16_t c, r;


    if (y + (tetro[t][rot][3][1] * 10) > 310)
        return 0;


    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        c = col + dx;
        r = row + dy;


        if (c < 0 || c >= 14 ||
            r < 0 || r >= 30 ||
            playfield[c][r])
        {
            return 0;
        }
    }

    return 1;
}

void LockTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int16_t base_col = (x - 10) / 10;
    int16_t base_row = (y - 10) / 10;
    int i;

    for (i = 0; i < 4; i++) {
        int8_t dx = tetro[t][rot][i][0];
        int8_t dy = tetro[t][rot][i][1];
        int c = base_col + dx;
        int r = base_row + dy;

        if (c >= 0 && c < 14 && r >= 0 && r < 30) {
            playfield[c][r] = 1; // mark playfield occupied

            if (locked_block_count < 400) {
                locked_blocks[locked_block_count].col = c;
                locked_blocks[locked_block_count].row = r;
                locked_blocks[locked_block_count].type = t;
                locked_block_count++;
            }
        }
    }
}
uint8_t CanMoveDown(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    return CanPlaceTetromino(t, x, y + 10, rot);
}

uint8_t CanMoveLeft(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    return CanPlaceTetromino(t, x - 10, y, rot);
}

uint8_t CanMoveRight(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    return CanPlaceTetromino(t, x + 10, y, rot);
}

uint8_t CanRotate(TetrominoType t, uint16_t x, uint16_t y, uint8_t new_rot)
{
    return CanPlaceTetromino(t, x, y, new_rot);
}

int ClearFullLines(void)
{
    int cleared_lines = 0;
    int row, col, r, c, i, full_line;


    for (row = 30 - 1; row >= 0; row--)
    {
        full_line = 1;
        for (col = 0; col < 14; col++)
        {
            if (playfield[col][row] == 0)
            {
                full_line = 0;
                break;
            }
        }

        if (full_line)
        {

            for (r = row; r > 0; r--)
            {
                for (c = 0; c < 14; c++)
                {
                    playfield[c][r] = playfield[c][r - 1];
                }
            }

            for (c = 0; c < 14; c++)
                playfield[c][0] = 0;


            i = 0;
            while (i < locked_block_count)
            {
                if (locked_blocks[i].row == row)
                {

                    locked_blocks[i] = locked_blocks[locked_block_count - 1];
                    locked_block_count--;
                }
                else
                {
                    if (locked_blocks[i].row < row)
                        locked_blocks[i].row++;
                    i++;
                }
            }

            cleared_lines++;
            row++; // re-check this row index after shift
        }
    }


    if (cleared_lines > 0)
    {
        RedrawPlayfield();
    }

    return cleared_lines;
}
