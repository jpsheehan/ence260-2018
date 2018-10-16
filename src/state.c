/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#include "state.h"

/**
 * Stores the state of the game.
 */
static uint8_t state;

/**
 * Sets the state of the game to _state.
 */
void setState(uint8_t _state)
{
  state = _state;
}

/**
 * Returns the state of the game.
 */
uint8_t getState(void)
{
  return state;
}