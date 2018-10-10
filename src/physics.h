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
PhysicsResult applyGravity(Game* game);

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 */
PhysicsResult rotateActivePiece(Game* game, uint8_t direction);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * Returns the number of lines cleared.
 */
uint8_t processLineClears(Game* game);

// MOVE THESE TO GRAPHICS
uint8_t frameBuffer[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];

void fillFramebuffer(Game *game);

#endif