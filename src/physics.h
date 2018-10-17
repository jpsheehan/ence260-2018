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
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 * 
 * @param game The game struct pointer.
 * @returns true if the tetromino is still in play.
 */
bool physics_applyGravity(Game* game);

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
 * Attempts to move the active piece in the specified direction.
 * If movement cannot occur, it will not be moved.
 * 
 * @param game The game struct pointer.
 * @param direction The direction to move the tetromino (LEFT or RIGHT).
 * @returns true if it succeeded.
 */
bool physics_moveActivePiece(Game* game, uint8_t direction);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * 
 * @param game The game struct pointer.
 * @returns The number of lines cleared.
 */
uint8_t physics_processLineClears(Game* game);

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

/**
 * Commits the current active tetromino to the stack.
 * After commiting the active piece, it checks to make sure that the game is not over.
 * 
 * @param game The game struct pointer.
 * @returns true if the game is not over.
 */
bool physics_commitActiveTetrominoToStack(Game* game);

/**
 * Performs a non-locking soft-drop of the active piece.
 * 
 * @param game The game struct pointer.
 */
void physics_softDrop(Game* game);

/**
 * Inserts n lines of junk at the bottom of the stack.
 * If this causes any part of the stack to collide with the "sky" this function returns false as the game is over.
 * To be fair to the player, the active piece is also moved up one piece.
 *
 * @param game The game struct pointer.
 * @param num_lines The number of lines of junk to insert.
 * @returns true if the game is not over.
 */
bool physics_insertJunk(Game* game, uint8_t num_lines);

/**
 * Gets the collision/draw data of a particular piece in a particular rotation.
 * 
 * @param piece The kind of piece you want.
 * @param rotation The rotation of the piece (ROTATE_0, ROTATE_90, ROTATE_180 or ROTATE_270).
 * @returns The collision/drawing data for each tetromino.
 */
Position* physics_getCollisionData(Piece piece, uint8_t rotation);

#endif