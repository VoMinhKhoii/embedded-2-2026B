#ifndef GAME_SESSION_H
#define GAME_SESSION_H

#include <stdint.h>
#include "tetromino_spawn.h"

typedef struct {
    TetrominoType tetromino;
    uint16_t originX;
    uint16_t originY;
    uint8_t rotation;
    uint8_t spawnNew;
    uint8_t spawn_delay_active;
    uint8_t spawn_counter;
    uint8_t spawn_ticks_per_spawn;
    uint8_t gravity_counter;
    uint8_t isPaused;
    uint8_t game_over;
} GameSession;

/* Wait for the initial start-button press on the splash screen. */
void GameSession_WaitForStart(void);
/* Configure the GPIO pins used for gameplay buttons. */
void GameSession_ConfigureControls(void);
/* Initialize gameplay state and draw the first playfield view. */
void GameSession_Begin(GameSession *session);
/* Run the main gameplay loop until reset or power-off. */
void GameSession_Run(GameSession *session);

#endif /* GAME_SESSION_H */
