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

/**
 * Checks if the position is free and returns one of WALL, STACK or EMPTY
 * Needs a more comprehensive test
 */
bool checkIfPositionFree(Game* game, Position pos) {
    return game->board[pos.y][pos.x] == EMPTY;
}

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
PhysicsResult applyGravity(Game* game)
{
    PhysicsResult result = {0};

    // switch (game->active_piece) {
    //     case I:
    //         if (game->active_orientation == ROTATE_0 ||
    //             game->active_orientation == ROTATE_180) {
    //             // I piece is vertical
                
    //         } else {
    //             // I piece is horizontal
    //         }
    //         break;
    //     case O:
    //         // no real orientation for this piece
    //         break;
    //     case T:
    //         switch (game->active_orientation) {
    //             case ROTATE_0:

    //                 break;
    //             case ROTATE_90:

    //                 break;
    //             case ROTATE_180:

    //                 break;
    //             case ROTATE_270:

    //                 break;
    //         }
    //         break;
    //     case S:
    //         if (game->active_orientation == ROTATE_0 ||
    //             game->active_orientation == ROTATE_180) {
    //             // S piece is horizontal
                
    //         } else {
    //             // S piece is vertical
    //         }
    //         break;
    //     case Z:
    //         if (game->active_orientation == ROTATE_0 ||
    //             game->active_orientation == ROTATE_180) {
    //             // Z piece is horizontal
                
    //         } else {
    //             // Z piece is vertical
    //         }
    //         break;
    //     case L:
    //         switch (game->active_orientation) {
    //             case ROTATE_0:

    //                 break;
    //             case ROTATE_90:

    //                 break;
    //             case ROTATE_180:

    //                 break;
    //             case ROTATE_270:

    //                 break;
    //         }
    //         break;
    //     case J:
    //         switch (game->active_orientation) {
    //             case ROTATE_0:

    //                 break;
    //             case ROTATE_90:

    //                 break;
    //             case ROTATE_180:

    //                 break;
    //             case ROTATE_270:

    //                 break;
    //         }
    //         break;
    // }

    // result.orientation = game->active_orientation;
    game->active_position = (Position){ game->active_position.x, game->active_position.y + 1};

    return result;
}

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 * Rotations are defined here: https://vignette.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
 * The I piece and the O piece are the only tetrominos which require special treatment when calculating their centre points during rotation
 * However the O piece doesn't change when rotated, so the I piece is the only piece that has a changing centre point
 */
PhysicsResult rotateActivePiece(Game* game, uint8_t direction)
{
    PhysicsResult result = {0};

    // change the orientation of the piece
    // switch (game->active_orientation) {
    //     case ROTATE_0:
    //         if (direction == CLOCKWISE) {
    //             result.orientation = ROTATE_90;
    //         } else {
    //             result.orientation = ROTATE_270;
    //         }
    //         break;
    //     case ROTATE_90:
    //         if (direction == CLOCKWISE) {
    //             result.orientation = ROTATE_180;
    //         } else {
    //             result.orientation = ROTATE_0;
    //         }
    //         break;
    //     case ROTATE_180:
    //         if (direction == CLOCKWISE) {
    //             result.orientation = ROTATE_270;
    //         } else {
    //             result.orientation = ROTATE_90;
    //         }
    //         break;
    //     case ROTATE_270:
    //         if (direction == CLOCKWISE) {
    //             result.orientation = ROTATE_180;
    //         } else {
    //             result.orientation = ROTATE_0;
    //         }
    //         break;
    // }

    // perform kicks
    // if (game->active_piece == O) {

    //     // O can't perform kicks
    //     result.position = (Position){ game->active_position.x, game->active_position.y };
    //     result.isLockedDown = false;

    // } else {

    //     Position *kickData = NULL;

    //     // perform kicks on other pieces
    //     switch (game->active_piece) {
    //         case I:
    //             // I has its own set of kicks
    //             kickData = &kickDataForI;
    //             break;
    //         case J:
    //         case L:
    //         case S:
    //         case Z:
    //         case T:
    //             // these pieces share their own kick data
    //             // kickData = kickDataForJLSTZ;
    //             break;
    //     }

    //     uint8_t row = game->active_orientation * 2 + (direction * 2 - 1) * (-1);
    //     uint8_t test = 0;
    //     for (; test < 5; test++) {

    //         // get the current position and add the relative test position to it
    //         Position pos = *(kickData + sizeof(Position) * 5 * row + sizeof(Position) * test);
    //         pos.x = pos.x + game->active_position.x;
    //         pos.y = pos.y + game->active_position.y;

    //         // if this position is free, this is the new position for the piece
    //         if (checkIfPositionFree(game, pos)) {
    //             result.position = pos;
    //             break;
    //         }
    //     }

    //     if (test == 5) {
    //         // could not move, set the position and orientation to the origingal values
    //         result.isLockedDown = false;
    //         result.orientation = game->active_orientation;
    //         result.position = game->active_position;
    //     }

    // }

    return result;
    
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