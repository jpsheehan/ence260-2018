#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "tetris.h"

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
void applyGravity(Board* board);

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 */
void rotateActivePiece(Board* board, uint8_t direction);

#endif