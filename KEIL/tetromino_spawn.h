#ifndef TETROMINO_SPAWN_H
#define TETROMINO_SPAWN_H

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

void InitializeTetrominoQueue(void);
/* Spawn the queued piece and update the next-piece preview. */
TetrominoType SpawnQueuedTetromino(uint16_t *originX, uint16_t *originY);

#endif /* TETROMINO_SPAWN_H */
