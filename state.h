#ifndef STATE_H_
#define STATE_H_

/**
 * The different states that the game can be in.
 */
typedef enum State {
    STARTUP,
    MAIN_MENU,
    SINGLE_PLAYER_GAME,
    MULTI_PLAYER_GAME
};

/**
 * Stores the state of the game.
 */
static State state;

/**
 * Sets the state of the game to _state.
 */
void setState(State _state);

/**
 * Returns the state of the game.
 */
State getState();

#endif