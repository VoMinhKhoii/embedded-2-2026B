#include "tetromino_spawn.h"
#include "tetris_draw.h"
#include "game_screens.h"
#include "EBI_LCD_Module.h"
#include <stdint.h>

#define TETROMINO_COUNT 7
extern volatile uint8_t  Timer3_cnt;
static uint32_t lcg_seed;

/* Store the preview piece so spawn and preview stay in sync. */
static TetrominoType _next_piece;
 
/* Use a small LCG to avoid pulling in a heavier RNG dependency. */
static uint32_t get_random(uint32_t modulus)
{
    lcg_seed = (1103515245 * lcg_seed + 12345) & 0x7FFFFFFF;
    return lcg_seed % modulus;
}

void InitializeTetrominoQueue(void)
{
    /* Seed from the timer so start-button timing affects the piece order. */
    lcg_seed = (uint32_t)Timer3_cnt;

    /* Draw the first preview immediately. */
    _next_piece = (TetrominoType)get_random(TETROMINO_COUNT);
    DrawNextTetromino(_next_piece);
}

TetrominoType SpawnQueuedTetromino(uint16_t *originX, uint16_t *originY)
{
    /* Spawn near the top-center of the playfield. */
    *originX = 5 * STONE_BLOCK;
    *originY = STONE_BLOCK;

    TetrominoType curr = _next_piece;
    DrawTetromino(curr, *originX, *originY, 0);

    _next_piece = (TetrominoType)get_random(TETROMINO_COUNT);
    DrawNextTetromino(_next_piece);

    return curr;
}
