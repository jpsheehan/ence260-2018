#include "physics.h"

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
void applyGravity(Game* game)
{

}

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 */
void rotateActivePiece(Game* game, uint8_t direction)
{

}

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * Returns the number of lines cleared.
 */
uint8_t processLineClears(Game* game)
{
    uint8_t num_clears = 0;
    
    uint8_t i = 0;
    for (; i < GAME_BOARD_HEIGHT; i++) {

        uint8_t j = 0;
        for (; j < GAME_BOARD_WIDTH; j++) {

            if (game->board[i][j] != 1) {
                break;
            }

        }

        if (j == GAME_BOARD_WIDTH) {
            num_clears++;
        }

    }
    return num_clears;
}