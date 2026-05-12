#include "tetromino_rotation.h"
#include "playfield.h"
#include "tetris_draw.h"
#include "game_screens.h"

/* Timer3 increments every 100 ms and is used for input throttling. */
extern volatile uint8_t Timer3_cnt;

/* Require at least one timer tick between rotations. */
static const uint8_t rotation_debounce_ticks = 1;
static uint8_t last_rotation_tick = 0;

void RotateTetrominoClockwiseWithKick(TetrominoType t,
                                      uint16_t *x,
                                      uint16_t *y,
                                      uint8_t *rot)
{
    uint8_t  new_rot;
    uint16_t tryX, tryY;

    /* Ignore repeated rotate presses inside the debounce window. */
    if ((uint8_t)(Timer3_cnt - last_rotation_tick) < rotation_debounce_ticks) {
        return;
    }

    new_rot = (uint8_t)((*rot + 1) & 3);
    tryY    = *y;

    /* Try the rotation in place first. */
    if (CanRotate(t, *x, tryY, new_rot)) {
        ClearTetromino(t, *x, tryY, *rot);
        *rot = new_rot;
    }
    else {
        /* Fall back to a one-cell wall kick to the left. */
        tryX = *x - STONE_BLOCK;
        if (CanRotate(t, tryX, tryY, new_rot)) {
            ClearTetromino(t, *x, tryY, *rot);
            *x   = tryX;
            *rot = new_rot;
        }
        else {
            /* If needed, try the symmetric kick to the right. */
            tryX = *x + STONE_BLOCK;
            if (CanRotate(t, tryX, tryY, new_rot)) {
                ClearTetromino(t, *x, tryY, *rot);
                *x   = tryX;
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

 
