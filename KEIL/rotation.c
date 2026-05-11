#include "rotation.h"
#include "playfield.h"
#include "tetris_draw.h"
#include "game_draw.h"      // for STONE_BLOCK
// Declare Timer3_cnt from EBI_LCD_Module.c (increments every 100ms)
extern volatile uint8_t Timer3_cnt;

// Debounce rotation: minimum 1 tick (100ms) between rotations
static const uint8_t rotation_debounce_ticks = 1; // 100ms
static uint8_t last_rotation_tick = 0;

void RotateTetrominoClockwise(TetrominoType t, uint16_t *x, uint16_t *y, uint8_t *rot)
{
    uint8_t  new_rot;
    uint16_t tryX, tryY;

    /* Debounce: skip if too soon since last rotation */
    if ((uint8_t)(Timer3_cnt - last_rotation_tick) < rotation_debounce_ticks) {
        return;
    }

    new_rot = (uint8_t)((*rot + 1) & 3);
    tryY    = *y;

    /* 1) Try rotating in place */
    if (CanRotate(t, *x, tryY, new_rot)) {
        /* all good, just clear & apply */
        ClearTetromino(t, *x, tryY, *rot);
        *rot = new_rot;
    }
    else {
        /* 2) Try kicking one cell left */
        tryX = *x - STONE_BLOCK;
        if (CanRotate(t, tryX, tryY, new_rot)) {
            ClearTetromino(t, *x, tryY, *rot);
            *x   = tryX;
            *rot = new_rot;
        }
        else {
            /* 3) Try kicking one cell right */
            tryX = *x + STONE_BLOCK;
            if (CanRotate(t, tryX, tryY, new_rot)) {
                ClearTetromino(t, *x, tryY, *rot);
                *x   = tryX;
                *rot = new_rot;
            }
            else {
                /* cannot rotate anywhere */
                return;
            }
        }
    }

    /* draw the newly rotated piece */
    DrawTetromino(t, *x, tryY, *rot);

    /* update debounce tick */
    last_rotation_tick = Timer3_cnt;
}

 