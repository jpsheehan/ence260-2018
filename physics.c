#include "physics.h"

/**
 * Applies gravity to the active piece.
 * If the active piece would collide with the stack then it is added to the stack before it is moved.
 */
PhysicsResult applyGravity(Game* game)
{
    PhysicsResult result = {0};

    switch (game->active_piece) {
        case I:
            if (game->active_orientation == ROTATE_0 ||
                game->active_orientation == ROTATE_180) {
                // I piece is vertical
                
            } else {
                // I piece is horizontal
            }
            break;
        case O:
            // no real orientation for this piece
            break;
        case T:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
        case S:
            if (game->active_orientation == ROTATE_0 ||
                game->active_orientation == ROTATE_180) {
                // S piece is horizontal
                
            } else {
                // S piece is vertical
            }
            break;
        case Z:
            if (game->active_orientation == ROTATE_0 ||
                game->active_orientation == ROTATE_180) {
                // Z piece is horizontal
                
            } else {
                // Z piece is vertical
            }
            break;
        case L:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
        case J:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
    }

    return result;
}

/**
 * Attempts to rotate the active piece by the amount specified.
 * If rotation cannot occur, nothing will happen.
 * A rotation will sometimes cause a kick to occur.
 */
PhysicsResult rotateActivePiece(Game* game, uint8_t direction)
{
    PhysicsResult result = {0};

    // change the orientation of the piece
    switch (game->active_orientation) {
        case ROTATE_0:
            if (direction == CLOCKWISE) {
                result.orientation = ROTATE_90;
            } else {
                result.orientation = ROTATE_270;
            }
            break;
        case ROTATE_90:
            if (direction == CLOCKWISE) {
                result.orientation = ROTATE_180;
            } else {
                result.orientation = ROTATE_0;
            }
            break;
        case ROTATE_180:
            if (direction == CLOCKWISE) {
                result.orientation = ROTATE_270;
            } else {
                result.orientation = ROTATE_90;
            }
            break;
        case ROTATE_270:
            if (direction == CLOCKWISE) {
                result.orientation = ROTATE_180;
            } else {
                result.orientation = ROTATE_0;
            }
            break;
    }

    // change the center position of the game piece
    switch (game->active_piece) {
        case I:
            switch (game->active_orientation) {
                case ROTATE_0:
                    if (direction == CLOCKWISE) {
                        result.position = (Position){ game->active_position.x + 1, game->active_position.y };
                    } else {
                        result.position = (Position){ game->active_position.x, game->active_position.y };
                    }
                    break;
                case ROTATE_90:
                    if (direction == CLOCKWISE) {
                        result.position = (Position){ game->active_position.x - 1, game->active_position.y - 1 };
                    } else {
                        result.position = (Position){ game->active_position.x - 1, game->active_position.y };
                    }
                    break;
                case ROTATE_180:
                    if (direction == CLOCKWISE) {
                        result.position = (Position){ game->active_position.x, game->active_position.y - 1 };
                    } else {
                        result.position = (Position){ game->active_position.x + 1, game->active_position.y + 1};
                    }
                    break;
                case ROTATE_270:
                    if (direction == CLOCKWISE) {
                        result.position = (Position){ game->active_position.x, game->active_position.y };
                    } else {
                        result.position = (Position){ game->active_position.x, game->active_position.y + 1};
                    }
                    break;
            }
            break;
        case O:
            // no real orientation for this piece
            // hence one cannot rotate it
            result.position = (Position){ game->active_position.x, game->active_position.y };
            break;
        case T:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
        case S:
            if (game->active_orientation == ROTATE_0 ||
                game->active_orientation == ROTATE_180) {
                // S piece is horizontal
                
            } else {
                // S piece is vertical
            }
            break;
        case Z:
            if (game->active_orientation == ROTATE_0 ||
                game->active_orientation == ROTATE_180) {
                // Z piece is horizontal
                
            } else {
                // Z piece is vertical
            }
            break;
        case L:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
        case J:
            switch (game->active_orientation) {
                case ROTATE_0:

                    break;
                case ROTATE_90:

                    break;
                case ROTATE_180:

                    break;
                case ROTATE_270:

                    break;
            }
            break;
    }

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