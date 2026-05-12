#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#define PLAYFIELD_LEFT   10
#define PLAYFIELD_RIGHT 150
#define PLAYFIELD_TOP    10
#define PLAYFIELD_BOTTOM 310
#include <stdint.h>
#include "tetromino_spawn.h"
#define PLAYFIELD_COLS 14
#define PLAYFIELD_ROWS 30
extern uint8_t playfield[PLAYFIELD_COLS][PLAYFIELD_ROWS];
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

#endif /* PLAYFIELD_H */
