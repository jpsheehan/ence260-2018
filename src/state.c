#include "state.h"

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