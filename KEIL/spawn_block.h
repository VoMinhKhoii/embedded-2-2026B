#ifndef TETRIS_SPAWN_H
#define TETRIS_SPAWN_H

#include <stdint.h>

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

#endif // TETRIS_SPAWN_H