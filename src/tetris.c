#include "tetris.h"
#include "physics.h"

/**
 * Returns a number from 0 - 6 (inclusive) that represents the next tetromino to spawned
 * IN FUTURE CONSIDER USING THE 7-BAG METHOD
 */
uint8_t getNextTetromino(void) {
    #if __AVR__
        return timer_get() % NUM_TETROMINOS;
    #else
        return rand() % NUM_TETROMINOS;
    #endif
}

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetromino can be spawned it returns true.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false
 */
bool spawnNextTetromino(Game* game) {
    game->active_piece = getNextTetromino();
    game->active_position = (Position){ DefaultSpawnPosition.x, DefaultSpawnPosition.y };
    game->active_orientation = ROTATE_0;

    if (testAbsolutePosition(game, game->active_position) == STACK) {
        commitActiveTetrominoToStack(game);
        return false;
    } else {
        return true;
    }
}

/**
 * To be called to initialise the game.
 */
void tetris_init(void) {
    DefaultSpawnPosition = (Position){1, 0};
}

/**
 * Starts a game of tetris.
 */
void playTetris(uint8_t num_players)
{
  
}
