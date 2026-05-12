#ifndef GAME_DRAW_H
#define GAME_DRAW_H

#include <stdint.h>
#define STONE_BLOCK 10
#define STONE_BLOCK 10
#define C_STONE_A 0x7BEF
#define C_STONE_B 0x6B4D
#define C_BLACK   0x0000
#define C_WHITE   0xFFFF
extern uint16_t currentScore;
extern uint8_t level;
extern uint8_t minutes;
extern uint8_t seconds;
extern char acString[32];
extern uint16_t currentHighScore;
void DrawStoneBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DisplayGameField(void);
void StartGameField(void);
void GameOverField(void);
void LeaderBoard(void);
/* Insert one result into the in-memory leaderboard in descending score order. */
void AddToLeaderboard(uint16_t score,
                      uint8_t level,
                      uint8_t minutes,
                      uint8_t seconds);

#endif /* GAME_DRAW_H */
