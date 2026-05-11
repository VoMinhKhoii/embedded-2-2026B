#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#define PLAYFIELD_LEFT   10
#define PLAYFIELD_RIGHT 150
#define PLAYFIELD_TOP    10
#define PLAYFIELD_BOTTOM 310
#include <stdint.h>
#include "spawn_block.h"
#define PLAYFIELD_COLS 14
#define PLAYFIELD_ROWS 30
extern uint8_t playfield[PLAYFIELD_COLS][PLAYFIELD_ROWS];
static uint16_t locked_count = 0;
#define MAX_LOCKED_BLOCKS 400

typedef struct {
    uint8_t col;          // column index (0 to PLAYFIELD_COLS-1)
    uint8_t row;          // row index (0 to PLAYFIELD_ROWS-1)
    TetrominoType type;   // block type/color
} LockedBlock;
static LockedBlock locked_blocks[MAX_LOCKED_BLOCKS];
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

#endif /* PLAYFIELD_H */