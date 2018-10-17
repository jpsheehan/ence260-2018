/**
 * state.c
 *
 * The state module provides functions and macros for handling the program state.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include "state.h"

/**
 * Stores the state of the game. We keep this static so that it cannot be directly accessed.
 * This may be desirable behaviour in the future.
 */
static uint8_t state;

/**
 * Sets the current state of the game.
 *
 * @param _state The state you wish to set (STATE_STARTUP, STATE_MENU, STATE_WON, STATE_LOST or STATE_SCORE).
 */
uint8_t state_get(void)
{
    return state;
}

/**
 * Gets the state of the game.
 *
 * @returns The current state of the game.
 */
void state_set(uint8_t _state)
{
    state = _state;
}