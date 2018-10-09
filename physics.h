#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdint.h>
#include "tetris.h"
#include <stdbool.h>

typedef struct physics_result_s {
    bool isLockedDown;
    Position position;
    Orientation orientation; 
} PhysicsResult;

/**
 * Wall kick data for J, L, S, T, Z minoes
 * from http://tetris.wikia.com/wiki/SRS
 * we take the negative of the y coordinate because the table describing the wall kicks
 * uses a positive y as going upwards
 */
Position kickDataForJLSTZ[8][5] = {
    { (Position){0, 0}, (Position){-1, 0}, (Position){-1, -1}, (Position){0, 2}, (Position){-1, 2} }, // 0 >> 1 or CLOCKWISE from ROTATE_0
    { (Position){0, 0}, (Position){1, 0}, (Position){1, -1}, (Position){0, 2}, (Position){1, 2} }, // 0 >> 3 or COUNTERCLOCKWISE from ROTATE_0
    { (Position){0, 0}, (Position){1, 0}, (Position){1, 1}, (Position){0, -2}, (Position){1, -2} }, // 1 >> 2 or CLOCKWISE from ROTATE_90
    { (Position){0, 0}, (Position){1, 0}, (Position){1, 1}, (Position){0, -2}, (Position){1, -2} }, // 1 >> 0 or COUNTERCLOCKWISE from ROTATE_90
    { (Position){0, 0}, (Position){1, 0}, (Position){1, -1}, (Position){0, 2}, (Position){1, 2} }, // 2 >> 3 or CLOCKWISE from ROTATE_180
    { (Position){0, 0}, (Position){-1, 0}, (Position){-1, -1}, (Position){0, 2}, (Position){-1, 2} }, // 2 >> 1 or COUNTERCLOCKWISE from ROTATE_180
    { (Position){0, 0}, (Position){-1, 0}, (Position){-1, 1}, (Position){0, -2}, (Position){-1, -2} }, // 3 >> 0 or CLOCKWISE from ROTATE_270
    { (Position){0, 0}, (Position){-1, 0}, (Position){-1, 1}, (Position){0, -2}, (Position){-1, -2} }, // 3 >> 2 or COUNTERCLOCKWISE from ROTATE_270
};

/**
 * Kick data for the I mino
 */
Position kickDataForI[8][5] = {
    { (Position){0, 0}, (Position){-2, 0}, (Position){1, 0}, (Position){-2, 1}, (Position){1, -2} }, // 0 >> 1 or CLOCKWISE from ROTATE_0
    { (Position){0, 0}, (Position){-1, 0}, (Position){2, 0}, (Position){-1, -2}, (Position){2, 1} }, // 0 >> 3 or COUNTERCLOCKWISE from ROTATE_0
    { (Position){0, 0}, (Position){-1, 0}, (Position){2, 0}, (Position){-1, -2}, (Position){2, 1} }, // 1 >> 2 or CLOCKWISE from ROTATE_90
    { (Position){0, 0}, (Position){2, 0}, (Position){-1, 0}, (Position){2, -1}, (Position){-1, 2} }, // 1 >> 0 or COUNTERCLOCKWISE from ROTATE_90
    { (Position){0, 0}, (Position){2, 0}, (Position){-1, 0}, (Position){2, -1}, (Position){-1, 2} }, // 2 >> 3 or CLOCKWISE from ROTATE_180
    { (Position){0, 0}, (Position){1, 0}, (Position){-2, 0}, (Position){1, 2}, (Position){-2, -1} }, // 2 >> 1 or COUNTERCLOCKWISE from ROTATE_180
    { (Position){0, 0}, (Position){1, 0}, (Position){-2, 0}, (Position){1, 2}, (Position){-2, -1} }, // 3 >> 0 or CLOCKWISE from ROTATE_270
    { (Position){0, 0}, (Position){-2, 0}, (Position){1, 0}, (Position){-2, 1}, (Position){1, -2} }, // 3 >> 2 or COUNTERCLOCKWISE from ROTATE_270
};

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

#endif