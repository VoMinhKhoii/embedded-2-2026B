// spawn_block.c

#include "spawn_block.h"
#include "tetris_draw.h"    // for DrawNextTetromino()
#include "game_draw.h"      // for STONE_BLOCK
#include "EBI_LCD_Module.h" // for extern volatile uint8_t Timer3_cnt
#include <stdint.h>

#define TETROMINO_COUNT 7
extern volatile uint8_t  Timer3_cnt;
// simple linear congruential generator (LCG) for pseudo-random numbers
static uint32_t lcg_seed;  // no fixed initializer any more

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
    // seed with Timer3_cnt so the user�s press timing randomizes it
    lcg_seed = (uint32_t)Timer3_cnt;

    // pick initial �next� piece
    _next_piece = (TetrominoType)get_random(TETROMINO_COUNT);
    DrawNextTetromino(_next_piece);
}

// spawn the queued piece, draw it at the top-center of the playfield,
// then immediately pick & preview the next piece
TetrominoType SpawnTetromino(uint16_t *originX, uint16_t *originY)
{
    *originX = 5 * STONE_BLOCK;
    *originY = STONE_BLOCK;

    TetrominoType curr = _next_piece;
    DrawTetromino(curr, *originX, *originY, 0);

    _next_piece = (TetrominoType)get_random(TETROMINO_COUNT);
    DrawNextTetromino(_next_piece);

    return curr;
}
