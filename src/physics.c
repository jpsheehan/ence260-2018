/**
 * physics.c
 * 
 * The physics module provides functions for movement, rotation and collision detection.
 * 
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include "physics.h"

/**
 * This serves as the drawing data and as the collision detection data.
 * This is interpreted from https://vignette.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
 * The centre point for the I piece is the second mino from the top or left.
 * The centre point for the O piece is the top left mino.
 * The centre point for all other pieces is as described in the image above.
 */
static Position collisionData[7][4][4] = {
    {
        { {-1, 0}, {0, 0}, {1, 0}, {2, 0} }, // I 0
        { {1, -1}, {1, 0}, {1, 1}, {1, 2} }, // I 90
        { {-1, 1}, {0, 1}, {1, 1}, {2, 1} }, // I 180
        { {0, -1}, {0, 0}, {0, 1}, {0, 2} }, // I 270
    },
    {
        { {-1, -1}, {-1, 0}, {0, 0}, {1, 0} }, // J 0
        { {1, -1}, {0, -1}, {0, 0}, {0, 1} }, // J 90
        { {-1, 0}, {0, 0}, {1, 0}, {1, 1} }, // J 180
        { {0, -1}, {0, 0}, {0, 1}, {-1, 1} }, // J 270
    },
    {
        { {1, -1}, {-1, 0}, {0, 0}, {1, 0} }, // L 0
        { {1, 1}, {0, -1}, {0, 0}, {0, 1} }, // L 90
        { {-1, 0}, {0, 0}, {1, 0}, {-1, 1} }, // L 180
        { {0, -1}, {0, 0}, {0, 1}, {-1, -1} }, // L 270
    },
    {
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, // O 0
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, // O 90
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, // O 180
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, // O 270
    },
    {
        { {-1, 0}, {0, 0}, {0, -1}, {1, -1} }, // S 0
        { {0, -1}, {0, 0}, {1, 0}, {1, 1} }, // S 90
        { {-1, 1}, {0, 1}, {0, 0}, {1, 0} }, // S 180
        { {-1, -1}, {-1, 0}, {0, 0}, {0, 1} }, // S 270
    },
    {
        { {0, 0}, {-1, 0}, {0, -1}, {1, 0} }, // T 0
        { {0, 0}, {0, -1}, {1, 0}, {0, 1} }, // T 90
        { {0, 0}, {1, 0}, {0, 1}, {-1, 0} }, // T 180
        { {0, 0}, {0, 1}, {-1, 0}, {0, -1} }, // T 270
    },
    {
        { {-1, -1}, {0, -1}, {0, 0}, {1, 0} }, // Z 0
        { {1, -1}, {1, 0}, {0, 0}, {0, 1} }, // Z 90
        { {-1, 0}, {0, 0}, {0, 1}, {1, 1} }, // Z 180
        { {0, -1}, {0, 0}, {-1, 0}, {-1, 1} }, // Z 270
    },
};

/**
 * Gets the collision/draw data of a particular piece in a particular rotation.
 * 
 * @param piece The kind of piece you want.
 * @param rotation The rotation of the piece (ROTATE_0, ROTATE_90, ROTATE_180 or ROTATE_270).
 * @returns The collision/drawing data for each tetromino.
 */
Position* physics_getCollisionData(Piece piece, uint8_t rotation)
{
    return collisionData[piece][rotation];
}

/**
 * Moves the active tetromino to the specified position and checks for collisions.
 * Possible return values are EMPTY, FLOOR, STACK and WALL.
 * 
 * @param game The game struct pointer.
 * @param absPos The absolute position of the tetromino to check.
 * @returns The kind of block that the active piece would intersect.
 */ 
uint8_t physics_testAbsolutePosition(Game* game, Position absPos)
{
    Position *collData = physics_getCollisionData(game->active_piece, game->active_orientation);

    uint8_t i;
    for (i = 0; i < NUM_MINOS_IN_PIECE; i++) {

        Position pos = { collData[i].x + absPos.x, collData[i].y + absPos.y };
        
        // check x bounds
        if (pos.x < 0 || pos.x >= GAME_BOARD_WIDTH) {
            return WALL;
        }

        // check y bounds
        if (pos.y >= GAME_BOARD_HEIGHT) {
            return FLOOR;
        }

        // we ignore checking for if the piece occupies space above the play area
        // but we still don't want to index game board with out of bounds indices
        if (pos.y >= 0) {
            uint8_t found = game->board[pos.y][pos.x];
            if (found != EMPTY) {
                return found;
            }
        }
    }
    return EMPTY;
}

/**
 * Moves the active tetromino to the specified position relative to its current position and checks for collisions.
 * Possible return values are EMPTY, FLOOR, STACK and WALL.
 * 
 * @param game The game struct pointer.
 * @param absPos The position of the tetromino relative to its current position` to check.
 * @returns The kind of block that the active piece would intersect.
 */ 
uint8_t physics_testRelativePosition(Game* game, Position relPos) {

    return physics_testAbsolutePosition(game, (Position){ game->active_position.x + relPos.x, game->active_position.y + relPos.y });
}

/**
 * Attempts to move the active piece in the specified direction.
 * If movement cannot occur, it will not be moved.
 * 
 * @param game The game struct pointer.
 * @param direction The direction to move the tetromino (LEFT or RIGHT).
 * @returns true if it succeeded.
 */
bool physics_moveActivePiece(Game* game, uint8_t direction)
{
    Position newPosition = { game->active_position.x + (direction == LEFT ? -1 : 1), game->active_position.y };

    uint8_t test = physics_testAbsolutePosition(game, newPosition);

    if (test == EMPTY) {
        // everything is ok! we can move the piece to the new position
        game->active_position.x = newPosition.x;
        game->active_position.y = newPosition.y;
        return true;
    } else {
        // we can't move the piece :(
        return false;
    }
}

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 * 
 * @param game The game struct pointer.
 * @returns true if the tetromino is still in play.
 */
bool physics_applyGravity(Game* game)
{
    Position newPosition = { game->active_position.x, game->active_position.y + 1 };

    uint8_t test = physics_testAbsolutePosition(game, newPosition);

    if (test == EMPTY) {
        // everything okay! let's move the piece to its new position
        game->active_position.x = newPosition.x;
        game->active_position.y = newPosition.y;
        return true;

    } else if (test == STACK || test == FLOOR) {
        // uh, oh! the piece collided with the floor or the stack, let's turn the active piece into part of the stack
        return false;
    }

    return true;
}


/**
 * Commits the current active tetromino to the stack.
 * After commiting the active piece, it checks to make sure that the game is not over.
 * 
 * @param game The game struct pointer.
 * @returns true if the game is not over.
 */
bool physics_commitActiveTetrominoToStack(Game* game)
{
    bool isGameOver = false;
    uint8_t i;
    for (i = 0; i < NUM_MINOS_IN_PIECE; i++) {
        Position relPos = physics_getCollisionData(game->active_piece, game->active_orientation)[i];
        int8_t x = game->active_position.x + relPos.x;
        int8_t y = game->active_position.y + relPos.y;
        if (y < 0) {
            isGameOver = true;
        } else {
            game->board[y][x] = STACK;
        }
    }
    return !isGameOver;
}

/**
 * Attempts to rotate the active piece either direction.
 * If rotation cannot occur, nothing will happen.
 * 
 * @param game The game struct pointer.
 * @param direction The direction to rotate (CLOCKWISE or COUNTERCLOCKWISE).
 * @returns true if successfully rotated.
 */
bool physics_rotateActivePiece(Game* game, uint8_t direction)
{
    // store the old rotation unless we neeed it
    Orientation oldOrientation = game->active_orientation;

    // change the orientation of the piece
    switch (game->active_orientation) {
        case ROTATE_0:
            if (direction == CLOCKWISE) {
                game->active_orientation = ROTATE_90;
            } else {
                game->active_orientation = ROTATE_270;
            }
            break;
        case ROTATE_90:
            if (direction == CLOCKWISE) {
                game->active_orientation = ROTATE_180;
            } else {
                game->active_orientation = ROTATE_0;
            }
            break;
        case ROTATE_180:
            if (direction == CLOCKWISE) {
                game->active_orientation = ROTATE_270;
            } else {
                game->active_orientation = ROTATE_90;
            }
            break;
        case ROTATE_270:
            if (direction == CLOCKWISE) {
                game->active_orientation = ROTATE_0;
            } else {
                game->active_orientation = ROTATE_180;
            }
            break;
    }

    // check to see if the rotation is valid (doesn't collide with anything)
    if (game->active_piece == O) {

        // O doesn't rotate
        return true;

    } else {

        if (physics_testAbsolutePosition(game, game->active_position) == EMPTY) {
            
            // if this orientation doesn't collide with anything, keep it there
            return true;

        } else {

            // rotation is not possible for this piece here. restore its orientation
            game->active_orientation = oldOrientation;
            return false;

        }

    }
      
}

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * 
 * @param game The game struct pointer.
 * @returns The number of lines cleared.
 */
uint8_t physics_processLineClears(Game* game)
{
    uint8_t num_clears = 0;
    
    // loop through each row
    uint8_t i;
    for (i = 0; i < GAME_BOARD_HEIGHT; i++) {
        
        // loop through each cell, checking that it is full (a part of the stack)
        uint8_t j;
        for (j = 0; j < GAME_BOARD_WIDTH; j++) {

            if (game->board[i][j] != STACK) {
                break;
            }

        }

        // if this row is full
        if (j == GAME_BOARD_WIDTH) {
            num_clears++;

            // we must now clear this row by shifting all rows above it down by 1

            // starting at the current row (the row to be cleared) and working our way up to the top of the board,
            // we must shift each k-1 row to the kth row position
            uint8_t k;
            for (k = i; k > 0; k--) {

                // for each cell in the previous row move it down one
                uint8_t j;
                for (j = 0; j < GAME_BOARD_WIDTH; j++) {
                    game->board[k][j] = game->board[k-1][j];
                }
            }

            // we must now clear the 0th row, because there cannot possibly be any stack there
            uint8_t j;
            for (j = 0; j < GAME_BOARD_WIDTH; j++) {
                game->board[0][j] = 0;
            }
        }
    }
    
    return num_clears;
}

/**
 * Performs a non-locking soft-drop of the active piece.
 * 
 * @param game The game struct pointer.
 */
void physics_applySoftDrop(Game* game)
{
    Position newPos = game->active_position;

    while (physics_testAbsolutePosition(game, newPos) == EMPTY) {
        newPos.y += 1;
    }
    newPos.y -= 1;

    game->active_position = newPos;
}

/**
 * Inserts n lines of junk at the bottom of the stack.
 * If this causes any part of the stack to collide with the "sky" this function returns false as the game is over.
 * To be fair to the player, the active piece is also moved up one piece.
 *
 * @param game The game struct pointer.
 * @param num_lines The number of lines of junk to insert.
 * @returns true if the game is not over.
 */
bool physics_insertJunk(Game* game, uint8_t num_lines)
{
    // generate a line of junk to insert. a line of junk has one "hole" in it
    uint8_t holePosition = rand() % GAME_BOARD_WIDTH;
    bool is_game_lost = false;

    // push the active piece up as well,
    // we don't want to be dicks about the fact that you're losing
    game->active_position.y -= 1;

    // insert n lines of junk
    uint8_t i;
    for (i = 0; i < num_lines; i++) {

        // first of all, shift everything in the entire stack up one
        uint8_t j;
        for (j = 0; j < GAME_BOARD_HEIGHT - 1; j++) {
            
            // shift each row up one
            uint8_t k;
            for (k = 0; k < GAME_BOARD_WIDTH; k++) {
                
                // if this is the first row, we check to see if any stack would be pushed
                // up out of the playfield
                if (j == 0) {
                    if (game->board[j][k] == STACK) {
                        is_game_lost = true;
                    }
                }

                game->board[j][k] = game->board[j+1][k];
            }

        }

        // insert the junk
        for (j = 0; j < GAME_BOARD_WIDTH; j++) {
            if (j == holePosition) {
                game->board[GAME_BOARD_HEIGHT-1][j] = EMPTY;
            } else {
                game->board[GAME_BOARD_HEIGHT-1][j] = STACK;
            }
        }

    }

    return !is_game_lost;

}