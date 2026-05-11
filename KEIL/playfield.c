#include "playfield.h"
#include "game_screens.h"
#include "tetris_draw.h"
uint8_t playfield[PLAYFIELD_COLS][PLAYFIELD_ROWS];
 
void ClearPlayfield(void)
{
    int c, r;
    /* Reset the logical board state. */
    for (c = 0; c < PLAYFIELD_COLS; c++)
        for (r = 0; r < PLAYFIELD_ROWS; r++)
            playfield[c][r] = 0;

    locked_block_count = 0;

    /* Clear the visible playfield area on the LCD. */
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

    /* Clear only the board interior before redrawing locked blocks. */
    LCD_BlankArea(
      PLAYFIELD_LEFT,
      PLAYFIELD_TOP,
      PLAYFIELD_COLS * STONE_BLOCK,
      PLAYFIELD_ROWS * STONE_BLOCK,
      C_BLACK
    );

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

    /* Let each occupied cell perform its own bounds check. */
    if (y + (tetro[t][rot][3][1] * STONE_BLOCK) > PLAYFIELD_BOTTOM)
        return 0;

    /* Validate all four cells of the active rotation. */
    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        c  = col + dx;
        r  = row + dy;

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
            playfield[c][r] = 1;

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

    /* Scan from the bottom upward so shifted rows are rechecked correctly. */
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
            for (r = row; r > 0; r--)
            {
                for (c = 0; c < PLAYFIELD_COLS; c++)
                {
                    playfield[c][r] = playfield[c][r - 1];
                }
            }
            for (c = 0; c < PLAYFIELD_COLS; c++)
                playfield[c][0] = 0;

            /* Mirror the logical row shift in the locked-block list. */
            i = 0;
            while (i < locked_block_count)
            {
                if (locked_blocks[i].row == row)
                {
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
            row++;
        }
    }

    if (cleared_lines > 0)
    {
        RedrawPlayfield();
    }

    return cleared_lines;
}
