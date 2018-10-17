/**
 * physics.h
 *
 * The physics module provides functions for movement, rotation and collision detection.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdint.h>
#include <stdbool.h>

#include "tetris.h"

/**
 * Macros for specifying the relative direction of rotation.
 */
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

/**
 * Macros for specifying the relative direction of movement.
 */
#define LEFT 0
#define RIGHT 1

/**
 * Macros for specifying the absolute rotation.
 */
#define ROTATE_0 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 *
 * @param game The game struct pointer.
 * @returns true if the tetromino is still in play.
 */
bool physics_applyGravity(Game* game);

/**
 * Gets the collision/draw data of a particular piece in a particular rotation.
 *
 * @param piece The kind of piece you want.
 * @param rotation The rotation of the piece (ROTATE_0, ROTATE_90, ROTATE_180 or ROTATE_270).
 * @returns The collision/drawing data for each tetromino.
 */
Position* physics_getCollisionData(Piece piece, uint8_t rotation);

/**
 * Attempts to move the active piece in the specified direction.
 * If movement cannot occur, it will not be moved.
 *
 * @param game The game struct pointer.
 * @param direction The direction to move the tetromino (LEFT or RIGHT).
 * @returns true if it succeeded.
 */
bool physics_moveActivePiece(Game* game, uint8_t direction);

/**
 * Attempts to rotate the active piece either direction.
 * If rotation cannot occur, nothing will happen.
 *
 * @param game The game struct pointer.
 * @param direction The direction to rotate (CLOCKWISE or COUNTERCLOCKWISE).
 * @returns true if successfully rotated.
 */
bool physics_rotateActivePiece(Game* game, uint8_t direction);

/**
 * Performs a non-locking soft-drop of the active piece.
 *
 * @param game The game struct pointer.
 */
void physics_softDrop(Game* game);

/**
 * Moves the active tetromino to the specified position and checks for collisions.
 * Possible return values are EMPTY, FLOOR, STACK and WALL.
 *
 * @param game The game struct pointer.
 * @param absPos The absolute position of the tetromino to check.
 * @returns The kind of block that the active piece would intersect.
 */
uint8_t physics_testAbsolutePosition(Game* game, Position absPos);

/**
 * Moves the active tetromino to the specified position relative to its current position and checks for collisions.
 * Possible return values are EMPTY, FLOOR, STACK and WALL.
 *
 * @param game The game struct pointer.
 * @param absPos The position of the tetromino relative to its current position` to check.
 * @returns The kind of block that the active piece would intersect.
 */
uint8_t physics_testRelativePosition(Game* game, Position relPos);

#endif