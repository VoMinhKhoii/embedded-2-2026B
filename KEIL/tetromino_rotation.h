#ifndef TETROMINO_ROTATION_H
#define TETROMINO_ROTATION_H

#include "tetromino_spawn.h"

void RotateTetrominoClockwiseWithKick(TetrominoType t,
                                      uint16_t *x,
                                      uint16_t *y,
                                      uint8_t *rot);

#endif /* TETROMINO_ROTATION_H */
