// tetris_draw.c
#include "tetris_draw.h"
#include "EBI_LCD_Module.h"
#include "game_screens.h"
#include "playfield.h"

#define STONE_BLOCK 10
 
// Colors in the same order as TetrominoType enum: I,J,L,O,S,T,Z
static const uint16_t colorOf[7] = {
    COLOR_I, COLOR_J, COLOR_L,
    COLOR_O, COLOR_S, COLOR_T,
    COLOR_Z
};
 
// dx,dy offsets for each piece (I=0?Z=6), each rotation 0?3, each of 4 blocks

void DrawTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int i;
    int8_t dx, dy;
    uint16_t px, py;
    uint16_t colColor = colorOf[t];

    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        px = x + dx * STONE_BLOCK;
        py = y + dy * STONE_BLOCK;
        LCD_BlankArea(px, py, STONE_BLOCK, STONE_BLOCK, colColor);
    }
}

void ClearTetromino(TetrominoType t, uint16_t x, uint16_t y, uint8_t rot)
{
    int i;
    int8_t dx, dy;
    uint16_t px, py;

    for (i = 0; i < 4; i++) {
        dx = tetro[t][rot][i][0];
        dy = tetro[t][rot][i][1];
        px = x + dx * STONE_BLOCK;
        py = y + dy * STONE_BLOCK;
        DrawPlayfieldCellBackground(px, py);
    }
}

void DrawNextTetromino(TetrominoType t)
{
    // carve out exactly the interior under ?NEXT? but inside the 10px stone border
    const int16_t innerX = NEXT_X + STONE_BLOCK;
    const int16_t innerY = NEXT_Y + NEXT_LABEL_H + 5;
    const int16_t innerW = NEXT_W  - 2*STONE_BLOCK;
    const int16_t innerH = NEXT_H  - NEXT_LABEL_H - 2*STONE_BLOCK;

    // 1) clear only that region
    LCD_BlankArea(
        innerX,
        innerY,
        innerW,
        innerH,
        RIGHT_PANEL_BG_COLOR
    );

    // 2) compute the spawn-rotation (rot=0) bounds of this tetromino
    {
        int8_t min_dx = tetro[t][0][0][0];
        int8_t max_dx = min_dx;
        int8_t min_dy = tetro[t][0][0][1];
        int8_t max_dy = min_dy;
        int i;
        for (i = 1; i < 4; i++) {
            int8_t dx = tetro[t][0][i][0];
            int8_t dy = tetro[t][0][i][1];
            if (dx < min_dx) min_dx = dx;
            if (dx > max_dx) max_dx = dx;
            if (dy < min_dy) min_dy = dy;
            if (dy > max_dy) max_dy = dy;
        }

        // convert to pixels
        const int16_t shapeW = (max_dx - min_dx + 1) * STONE_BLOCK;
        const int16_t shapeH = (max_dy - min_dy + 1) * STONE_BLOCK;

        // center within the cleared area, then offset by -min_dx/-min_dy
        const int16_t drawX = innerX
                            + (innerW - shapeW) / 2
                            - min_dx * STONE_BLOCK;
        const int16_t drawY = innerY
                            + (innerH - shapeH) / 2
                            - min_dy * STONE_BLOCK;

        // 3) draw the next piece (rotation=0)
        DrawTetromino(t, drawX, drawY, 0);
    }

    /* Restore the label because the preview clear area now overlaps it. */
    LCD_PutString(179, 10, (uint8_t *)"NEXT", 0x0000, RIGHT_PANEL_BG_COLOR);
}
void DrawBlock(uint16_t x, uint16_t y, TetrominoType type)
{
    // Draw one 10x10 square block with color matching type
    static const uint16_t colors[] = {
        COLOR_I, COLOR_J, COLOR_L, COLOR_O,
        COLOR_S, COLOR_T, COLOR_Z
    };

    uint16_t color = (type >= TETROMINO_COUNT) ? C_WHITE : colors[type];

    LCD_BlankArea(x, y, STONE_BLOCK, STONE_BLOCK, color);
}
