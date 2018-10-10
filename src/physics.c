#include "physics.h"
#include <stdlib.h>

/**
 * Wall kick data for J, L, S, T, Z minoes
 * from http://tetris.wikia.com/wiki/SRS
 * we take the negative of the y coordinate because the table describing the wall kicks
 * uses a positive y as going upwards
 */
static Position kickDataForJLSTZ[8][5] = {
    { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} }, // 0 >> 1 or CLOCKWISE from ROTATE_0
    { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} }, // 0 >> 3 or COUNTERCLOCKWISE from ROTATE_0
    { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} }, // 1 >> 2 or CLOCKWISE from ROTATE_90
    { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} }, // 1 >> 0 or COUNTERCLOCKWISE from ROTATE_90
    { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} }, // 2 >> 3 or CLOCKWISE from ROTATE_180
    { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} }, // 2 >> 1 or COUNTERCLOCKWISE from ROTATE_180
    { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} }, // 3 >> 0 or CLOCKWISE from ROTATE_270
    { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} }, // 3 >> 2 or COUNTERCLOCKWISE from ROTATE_270
};

/**
 * Kick data for the I mino
 */
static Position kickDataForI[8][5] = {
    { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} }, // 0 >> 1 or CLOCKWISE from ROTATE_0
    { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} }, // 0 >> 3 or COUNTERCLOCKWISE from ROTATE_0
    { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} }, // 1 >> 2 or CLOCKWISE from ROTATE_90
    { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} }, // 1 >> 0 or COUNTERCLOCKWISE from ROTATE_90
    { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} }, // 2 >> 3 or CLOCKWISE from ROTATE_180
    { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} }, // 2 >> 1 or COUNTERCLOCKWISE from ROTATE_180
    { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} }, // 3 >> 0 or CLOCKWISE from ROTATE_270
    { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} }, // 3 >> 2 or COUNTERCLOCKWISE from ROTATE_270
};

// this serves as the drawing data and as the collision detection data.
static Position drawData[7][4][4] = {
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
        { {-1, 0}, {0, 0}, {1, 0}, {1, -1} }, // L 180
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
        { {-1, -1}, {-1, 0}, {0, 0}, {1, 0} }, // Z 0
        { {1, -1}, {1, 0}, {0, 0}, {0, 1} }, // Z 90
        { {-1, 0}, {0, 0}, {0, 1}, {1, 1} }, // Z 180
        { {0, -1}, {0, 0}, {-1, 0}, {-1, 1} }, // Z 270
    },
};

void fillFramebuffer(Game *game)
{   
    // copy the stack data and clear the frameBuffer at the same time
    uint8_t i = 0;
    for (; i < GAME_BOARD_HEIGHT; i++) {
        uint8_t j = 0;
        for (; j < GAME_BOARD_WIDTH; j++) {
            if (game->board[i][j]) {
                frameBuffer[i][j] = STACK;
            } else {
                frameBuffer[i][j] = EMPTY;
            }
        }
    }

    // make it easier to access thhe position of the active piece
    Position* absPos = &game->active_position;

    // draw the piece data
    Position *posData = drawData[game->active_piece][game->active_orientation];

    for (i = 0; i < 4; i++) {
        Position relPos = posData[i];
        int8_t x = absPos->x + relPos.x;
        int8_t y = absPos->y + relPos.y;

        if (x >= 0 && y >= 0 && x < GAME_BOARD_WIDTH && y < GAME_BOARD_HEIGHT) {
            frameBuffer[y][x] = ACTIVE;
        }
    }
    
}

uint8_t testAbsolutePosition(Game* game, Position absPos)
{
    Position *collData = drawData[game->active_piece][game->active_orientation];

    uint8_t i = 0;
    for (; i < NUM_MINOS_IN_PIECE; i++) {

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
 * Checks if the current piece, in its current orientation could occupy the relative position pos.
 * Returns the kind of clipping that would occur: EMPTY, WALL, FLOOR or STACK
 */
uint8_t testRelativePosition(Game* game, Position pos) {

    return testAbsolutePosition(game, (Position){ game->active_position.x + pos.x, game->active_position.y + pos.y });
}

/**
 * Attempts to move the active piece in the specified direction.
 * Returns true if it succeeded.
 */
bool moveActivePiece(Game* game, uint8_t direction)
{
    Position newPosition = { game->active_position.x + (direction == LEFT ? -1 : 1), game->active_position.y };

    uint8_t test = testAbsolutePosition(game, newPosition);

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
 * It spawns a new piece if the active piece collided with the stack or floor.
 * Returns true if it was added to the stack
 */
bool applyGravity(Game* game)
{
    Position newPosition = { game->active_position.x, game->active_position.y + 1 };

    uint8_t test = testAbsolutePosition(game, newPosition);
    uint8_t i = 0;

    switch (test) {
        case EMPTY:
            // everything okay! let's move the piece to its new position
            game->active_position.x = newPosition.x;
            game->active_position.y = newPosition.y;
            return false;
        case FLOOR:
        case STACK:
            // uh, oh! the piece collided with the floor or the stack, let's turn the active piece into part of the stack
            for (; i < NUM_MINOS_IN_PIECE; i++) {
                Position relPos = drawData[game->active_piece][game->active_orientation][i];
                uint8_t x = game->active_position.x + relPos.x;
                uint8_t y = game->active_position.y + relPos.y;
                game->board[y][x] = STACK;
            }
            spawnNextTetromino(game);
            return true;
    }

    return false;
}

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 * Rotations are defined here: https://vignette.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
 * The I piece and the O piece are the only tetrominos which require special treatment when calculating their centre points during rotation
 * However the O piece doesn't change when rotated, so the I piece is the only piece that has a changing centre point
 */
bool rotateActivePiece(Game* game, uint8_t direction)
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
                game->active_orientation = ROTATE_180;
            } else {
                game->active_orientation = ROTATE_0;
            }
            break;
    }

    // perform kicks
    if (game->active_piece == O) {

        // O can't perform kicks, so we say that the rotation has occurred
        return true;

    } else {

        Position *kickData = NULL;

        // perform kicks on other pieces
        switch (game->active_piece) {
            case I:
                // I has its own set of kicks
                kickData = kickDataForI;
                break;
            case J:
            case L:
            case S:
            case Z:
            case T:
                // these pieces share their own kick data
                kickData = kickDataForJLSTZ;
                break;
        }

        // get the row of kickData to loop through
        Position *kickDataArray = (kickData + (2 * oldOrientation + direction) * 5 * sizeof(Position));

        uint8_t i = 0;
        for (; i < 5; i++) {

            // get the absolute position of the new position
            Position testPosition = *(kickDataArray + i * sizeof(Position));
            testPosition.x = testPosition.x + game->active_position.x;
            testPosition.y = testPosition.y + game->active_position.y;

            // if this position is free, this is the new position for the piece
            if (testAbsolutePosition(game, testPosition) == EMPTY) {
                game->active_position = testPosition;
                return true;
            }
        }

        // we could not find a suitable rotation for the piece
        // restore the old orientation
        game->active_orientation = oldOrientation;
        return false;

    }
      
}

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * Returns the number of lines cleared.
 */
uint8_t processLineClears(Game* game)
{
    uint8_t num_clears = 0;
    
    // loop through each row
    uint8_t i = 0;
    for (; i < GAME_BOARD_HEIGHT; i++) {
        
        // loop through each cell, checking that it is full (a part of the stack)
        uint8_t j = 0;
        for (; j < GAME_BOARD_WIDTH; j++) {

            if (game->board[i][j] != 1) {
                break;
            }

        }

        // if this row is full
        if (j == GAME_BOARD_WIDTH) {
            num_clears++;

            // we must now clear this row by shifting all rows above it down by 1

            // starting at the current row (the row to be cleared) and working our way up to the top of the board,
            // we must shift each k-1 row to the kth row position
            int k = i;
            for (; k > 0; k--) {

                // for each cell in the previous row move it down one
                int j = 0;
                for (; j < GAME_BOARD_WIDTH; j++) {
                    game->board[k][j] = game->board[k-1][j];
                }
            }

            // we must now clear the 0th row, because there cannot possibly be any stack there
            int j = 0;
            for (; j < GAME_BOARD_WIDTH; j++) {
                game->board[0][j] = 0;
            }
        }
    }
    
    return num_clears;
}