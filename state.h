#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

/**
 * The different states that the game can be in.
 */
enum state_e {
    STARTUP = 0,
    MAIN_MENU = 1,
    SINGLE_PLAYER_GAME = 2,
    MULTI_PLAYER_GAME = 3
};

/**
 * Stores the state of the game.
 */
static uint8_t state;

/**
 * Sets the state of the game to _state.
 */
void setState(uint8_t _state);

/**
 * Returns the state of the game.
 */
uint8_t getState(void);

#endif