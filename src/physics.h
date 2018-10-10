#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tetris.h"

typedef struct physics_result_s {
    bool isLockedDown;
    Position position;
    Orientation orientation; 
} PhysicsResult;

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
bool applyGravity(Game* game);

/**
 * Attempts to rotate the active piece either CLOCKWISE or COUNTERCLOCKWISE.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 * It will return true if successfully rotated
 */
bool rotateActivePiece(Game* game, uint8_t direction);

/**
 * Attempts to move the active piece in the specified direction.
 * Returns true if it succeeded.
 */
bool moveActivePiece(Game* game, uint8_t direction);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * Returns the number of lines cleared.
 */
uint8_t processLineClears(Game* game);

uint8_t testAbsolutePosition(Game* game, Position absPos);

uint8_t testRelativePosition(Game* game, Position relPos);

bool commitActiveTetrominoToStack(Game* game);

/**
 * Performs a non-locking soft-drop of the active piece
 */
void softDrop(Game* game);

// MOVE THESE TO GRAPHICS
uint8_t frameBuffer[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];

void fillFramebuffer(Game *game);

#endif