#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdint.h>
#include "tetris.h"

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
void applyGravity(Game* game);

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 */
void rotateActivePiece(Game* game, uint8_t direction);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * Returns the number of lines cleared.
 */
uint8_t processLineClears(Game* game);

#endif