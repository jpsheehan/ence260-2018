/**
 * state.h
 *
 * The state module provides functions and macros for handling the program state.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>

/**
 * The different states that the game can be in.
 */
#define STATE_STARTUP 0 // displaying the startup screen
#define STATE_MENU 1    // displaying the player selection screen
#define STATE_WON 2     // displaying the victory screen
#define STATE_LOST 3    // displaying the loss screen
#define STATE_SCORE 4   // displaying the score

/**
 * Gets the state of the game.
 *
 * @returns The current state of the game.
 */
uint8_t state_get(void);

/**
 * Sets the current state of the game.
 *
 * @param _state The state you wish to set (STATE_STARTUP, STATE_MENU, STATE_WON, STATE_LOST or STATE_SCORE).
 */
void state_set(uint8_t _state);

#endif