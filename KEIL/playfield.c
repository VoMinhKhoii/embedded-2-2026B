#include "playfield.h"
#include "game_draw.h"
#include "tetris_draw.h"
uint8_t playfield[PLAYFIELD_COLS][PLAYFIELD_ROWS];
 
void ClearPlayfield(void)
{
    int c, r;
    /* 1) Clear the logical playfield */
    for (c = 0; c < PLAYFIELD_COLS; c++)
        for (r = 0; r < PLAYFIELD_ROWS; r++)
            playfield[c][r] = 0;

    locked_block_count = 0;   // reset the real block-count

    /* 2) Visually clear the playfield region */
    LCD_BlankArea(
      PLAYFIELD_LEFT,
      PLAYFIELD_TOP,
      PLAYFIELD_COLS * STONE_BLOCK,
      PLAYFIELD_ROWS * STONE_BLOCK,
      C_BLACK
    );
}
void RedrawPlayfield(void)
{
    int i;

    /* 1) Blank only the playfield interior */
    LCD_BlankArea(
      PLAYFIELD_LEFT,
      PLAYFIELD_TOP,
      PLAYFIELD_COLS * STONE_BLOCK,
      PLAYFIELD_ROWS * STONE_BLOCK,
      C_BLACK
    );

    /* 2) Draw every locked block back */
    for (i = 0; i < locked_block_count; i++)
    {
        LockedBlock *lb = &locked_blocks[i];
        DrawBlock(
          PLAYFIELD_LEFT + lb->col * STONE_BLOCK,
          PLAYFIELD_TOP  + lb->row * STONE_BLOCK,
          lb->type
        );
    }
}
uint8_t CanPlaceTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int16_t col = ((int16_t)x - PLAYFIELD_LEFT) / STONE_BLOCK;
    int16_t row = ((int16_t)y - PLAYFIELD_TOP ) / STONE_BLOCK;
    int i;
    int8_t dx, dy;
    int16_t c, r;

    /* No global x-bounds check here�let each cell�s c<0 or c>=COLS reject it */
    if (y + (tetro[t][rot][3][1] * STONE_BLOCK) > PLAYFIELD_BOTTOM)
        return 0;

    /* Check each of the 4 blocks */
    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        c  = col + dx;
        r  = row + dy;

        /* Reject if outside or occupied */
        if (c < 0 || c >= PLAYFIELD_COLS ||
            r < 0 || r >= PLAYFIELD_ROWS ||
            playfield[c][r])
        {
            return 0;
        }
    }

    return 1;
}

void LockTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int16_t base_col = (x - PLAYFIELD_LEFT) / STONE_BLOCK;
    int16_t base_row = (y - PLAYFIELD_TOP) / STONE_BLOCK;
    int i;

    for (i = 0; i < 4; i++) {
        int8_t dx = tetro[t][rot][i][0];
        int8_t dy = tetro[t][rot][i][1];
        int c = base_col + dx;
        int r = base_row + dy;

        if (c >= 0 && c < PLAYFIELD_COLS && r >= 0 && r < PLAYFIELD_ROWS) {
            playfield[c][r] = 1;  // mark playfield occupied

            if (locked_block_count < MAX_LOCKED_BLOCKS) {
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

    /* 1) Scan bottom->top */
    for (row = PLAYFIELD_ROWS - 1; row >= 0; row--)
    {
        full_line = 1;
        for (col = 0; col < PLAYFIELD_COLS; col++)
        {
            if (playfield[col][row] == 0)
            {
                full_line = 0;
                break;
            }
        }

        if (full_line)
        {
            /* 2) Shift every row above down one */
            for (r = row; r > 0; r--)
            {
                for (c = 0; c < PLAYFIELD_COLS; c++)
                {
                    playfield[c][r] = playfield[c][r - 1];
                }
            }
            /* 3) Clear the new top row */
            for (c = 0; c < PLAYFIELD_COLS; c++)
                playfield[c][0] = 0;

            /* 4) Update locked_blocks[] */
            i = 0;
            while (i < locked_block_count)
            {
                if (locked_blocks[i].row == row)
                {
                    /* remove this block */
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
            row++;  // re-check this row index after shift
        }
    }

    /* 5) If anything was removed, redraw playfield interior */
    if (cleared_lines > 0)
    {
        RedrawPlayfield();
    }

    return cleared_lines;
}