#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

/**
 * The different states that the game can be in.
 */
#define STATE_STARTUP 0
#define STATE_MENU 1
#define STATE_WON 2
#define STATE_LOST 3
#define STATE_SCORE 4

/**
 * Sets the state of the game to _state.
 */
void setState(uint8_t _state);

/**
 * Returns the state of the game.
 */
uint8_t getState(void);

#endif