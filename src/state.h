#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

/**
 * The different states that the game can be in.
 */
#define STATE_STARTUP 0
#define STATE_MENU 1
#define STATE_1P_GAME 2
#define STATE_2P_GAME 3

/**
 * Sets the state of the game to _state.
 */
void setState(uint8_t _state);

/**
 * Returns the state of the game.
 */
uint8_t getState(void);

#endif