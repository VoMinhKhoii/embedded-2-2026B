#include "playfield.h"
#include "EBI_LCD_Module.h"
#include "game_screens.h"
#include "tetris_draw.h"
#define MAX_LOCKED_BLOCKS 400

typedef struct {
    uint8_t col;
    uint8_t row;
    TetrominoType type;
} LockedBlock;

uint8_t playfield[PLAYFIELD_COLS][PLAYFIELD_ROWS];
static LockedBlock locked_blocks[MAX_LOCKED_BLOCKS];
static int locked_block_count = 0;

static uint16_t GetPlayfieldCellColor(int col, int row)
{
    return ((col + row) & 1) ? PLAYFIELD_ALT_BG_COLOR : UI_BG_COLOR;
}

static int16_t PixelToPlayfieldCol(uint16_t x)
{
    return ((int16_t)x - PLAYFIELD_LEFT) / STONE_BLOCK;
}

static int16_t PixelToPlayfieldRow(uint16_t y)
{
    return ((int16_t)y - PLAYFIELD_TOP) / STONE_BLOCK;
}

static uint16_t PlayfieldCellX(int16_t col)
{
    return PLAYFIELD_LEFT + col * STONE_BLOCK;
}

static uint16_t PlayfieldCellY(int16_t row)
{
    return PLAYFIELD_TOP + row * STONE_BLOCK;
}

static uint8_t IsRowFull(int row)
{
    int col;

    for (col = 0; col < PLAYFIELD_COLS; col++) {
        if (playfield[col][row] == 0) {
            return 0;
        }
    }

    return 1;
}

static void ShiftPlayfieldRowsDownFrom(int row)
{
    int r, c;

    for (r = row; r > 0; r--) {
        for (c = 0; c < PLAYFIELD_COLS; c++) {
            playfield[c][r] = playfield[c][r - 1];
        }
    }

    for (c = 0; c < PLAYFIELD_COLS; c++) {
        playfield[c][0] = 0;
    }
}

static void RemoveLockedBlockAt(int index)
{
    locked_blocks[index] = locked_blocks[locked_block_count - 1];
    locked_block_count--;
}

static void ShiftLockedBlocksAfterRowClear(int row)
{
    int i = 0;

    while (i < locked_block_count) {
        if (locked_blocks[i].row == row) {
            RemoveLockedBlockAt(i);
        } else {
            if (locked_blocks[i].row < row) {
                locked_blocks[i].row++;
            }
            i++;
        }
    }
}

void DrawPlayfieldBackground(void)
{
    int c, r;

    for (c = 0; c < PLAYFIELD_COLS; c++) {
        for (r = 0; r < PLAYFIELD_ROWS; r++) {
            LCD_BlankArea(PlayfieldCellX(c),
                          PlayfieldCellY(r),
                          STONE_BLOCK,
                          STONE_BLOCK,
                          GetPlayfieldCellColor(c, r));
        }
    }
}

void DrawPlayfieldCellBackground(uint16_t x, uint16_t y)
{
    int col = PixelToPlayfieldCol(x);
    int row = PixelToPlayfieldRow(y);

    if (col < 0 || col >= PLAYFIELD_COLS || row < 0 || row >= PLAYFIELD_ROWS) {
        return;
    }

    LCD_BlankArea(x, y, STONE_BLOCK, STONE_BLOCK, GetPlayfieldCellColor(col, row));
}
 
void ClearPlayfield(void)
{
    int c, r;
    /* Reset the logical board state. */
    for (c = 0; c < PLAYFIELD_COLS; c++)
        for (r = 0; r < PLAYFIELD_ROWS; r++)
            playfield[c][r] = 0;

    locked_block_count = 0;

    /* Redraw the visible playfield area using the checker background. */
    DrawPlayfieldBackground();
}
void RedrawPlayfield(void)
{
    int i;

    /* Clear only the board interior before redrawing locked blocks. */
    DrawPlayfieldBackground();

    for (i = 0; i < locked_block_count; i++)
    {
        LockedBlock *lb = &locked_blocks[i];
        DrawBlock(
          PlayfieldCellX(lb->col),
          PlayfieldCellY(lb->row),
          lb->type
        );
    }
}
uint8_t CanPlaceTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int16_t col = PixelToPlayfieldCol(x);
    int16_t row = PixelToPlayfieldRow(y);
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
    int16_t base_col = PixelToPlayfieldCol(x);
    int16_t base_row = PixelToPlayfieldRow(y);
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
    return CanPlaceTetromino(t, x, y + STONE_BLOCK, rot);
}

uint8_t CanMoveLeft(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    return CanPlaceTetromino(t, x - STONE_BLOCK, y, rot);
}

uint8_t CanMoveRight(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    return CanPlaceTetromino(t, x + STONE_BLOCK, y, rot);
}

uint8_t CanRotate(TetrominoType t, uint16_t x, uint16_t y, uint8_t new_rot)
{
    return CanPlaceTetromino(t, x, y, new_rot);
}

int ClearFullLines(void)
{
    int cleared_lines = 0;
    int row;

    /* Scan from the bottom upward so shifted rows are rechecked correctly. */
    for (row = PLAYFIELD_ROWS - 1; row >= 0; row--)
    {
        if (IsRowFull(row))
        {
            ShiftPlayfieldRowsDownFrom(row);
            ShiftLockedBlocksAfterRowClear(row);
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
