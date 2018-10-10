#include "tetris.h"
#include "physics.h"

/**
 * Returns a number from 0 - 6 (inclusive) that represents the next tetromino to spawned
 * IN FUTURE CONSIDER USING THE 7-BAG METHOD
 */
uint8_t getNextTetromino(void) {
    #ifdef __AVR__
        return timer_get() % NUM_TETROMINOS;
    #else
        return rand() % NUM_TETROMINOS;
    #endif
}


/**
 * spawns a particular tetromino.
 */
void spawnTetromino(Game* game, Piece piece)
{
    game->active_piece = piece;
    game->active_position = (Position){ DefaultSpawnPosition.x, DefaultSpawnPosition.y };
    game->active_orientation = ROTATE_0;
}

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetromino can be spawned it returns true.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false
 */
bool spawnNextTetromino(Game* game) {

    spawnTetromino(game, getNextTetromino());
    game->has_held_this_turn = false;
    if (testAbsolutePosition(game, game->active_position) == STACK) {
        commitActiveTetrominoToStack(game);
        return false;
    } else {
        return true;
    }
}

/**
 * Attempts to hold the active piece.
 * Returns true if successful.
 */
bool holdPiece(Game* game)
{
    if (game->has_held_this_turn == false) {
        Piece tempPiece = game->held_piece;
        game->held_piece = game->active_piece;
        spawnTetromino(game, tempPiece);
        game->has_held_this_turn = true;
        return true;
    } else {
        return false;
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
