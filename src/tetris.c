#include "tetris.h"

/**
 * Generates the next (or first) tetromino and spawns it.
 */
void spawnNextTetromino(Game* game) {
    game->active_piece = timer_get() % 7;
    game->active_position = DefaultSpawnPosition;
    game->active_orientation = ROTATE_0;
}

/**
 * Starts a game of tetris.
 */
void playTetris(uint8_t num_players)
{
  
}
