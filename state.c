#include "state.h"

/**
 * Sets the state of the game to _state.
 */
void setState(State _state)
{
  state = _state;
}

/**
 * Returns the state of the game.
 */
State getState()
{
  return state;
}