/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"
#include "pacer.h"
#include "ir_uart.h"
#include "timer.h"
#include "tinygl.h"

#include "tetris.h"
#include "physics.h"
#include "graphics.h"
#include "randomiser.h"

/**
 * spawns a particular tetromino.
 */
void spawnTetromino(Game* game, Piece piece)
{
    game->piece = piece;
    game->position = (Position){ DefaultSpawnPosition.x, DefaultSpawnPosition.y };
    game->orientation = ROTATE_0;
}

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetromino can be spawned it returns true.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false
 */
bool spawnNextTetromino(Game* game) {

    spawnTetromino(game, randomiser_getNextTetromino());
    game->has_held_this_turn = false;
    if (physics_testAbsolutePosition(game, game->position) == STACK) {
        physics_commitActiveTetrominoToStack(game);
        return false;
    } else {
        return true;
    }
}

/**
 * Attempts to hold the active piece.
 * Returns true if successful.
 */
bool holdPiece(Game* game)
{
    if (game->has_held_this_turn == false) {
        Piece tempPiece = game->held_piece;
        game->held_piece = game->piece;
        if (tempPiece == NONE) {
            spawnTetromino(game, randomiser_getNextTetromino());
        } else {
            spawnTetromino(game, tempPiece);
        }
        game->has_held_this_turn = true;
        return true;
    } else {
        return false;
    }
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
        Position relPos = physics_getCollisionData(game->piece, game->orientation)[i];
        int8_t x = game->position.x + relPos.x;
        int8_t y = game->position.y + relPos.y;
        if (y < 0) {
            isGameOver = true;
        } else {
            game->board[y][x] = STACK;
        }
    }
    return !isGameOver;
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
    game->position.y -= 1;

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

/**
 * To be called to initialise the game.
 */
void tetris_init(void) {
    DefaultSpawnPosition = (Position){1, 0};
    randomiser_generateSevenBag();
}


// Allocates a new game on the heap
Game* newGame(void)
{
    // initialise a new game object
    Game* game = malloc(sizeof(Game));
    game->piece = I;
    game->orientation = ROTATE_0;
    game->position = DefaultSpawnPosition;
    game->score = 0;
    game->held_piece = NONE;
    game->has_held_this_turn = false;

    // clear the game board
    uint8_t i;
    for (i = 0; i < GAME_BOARD_HEIGHT; i++) {
        uint8_t j;
        for (j = 0; j < GAME_BOARD_WIDTH; j++) {
            game->board[i][j] = EMPTY;
            game->framebuffer[i][j] = EMPTY;
        }
    }

    randomiser_generateSevenBag();
    spawnNextTetromino(game);
    graphics_fillFramebuffer(game);
    return game;
}

// frees the memory used by a game
void destroyGame(Game* game)
{
    free(game);
}


/**
 * Starts a game of tetris.
 */
uint8_t playTetris(uint8_t num_players)
{
    uint8_t receivedChar;
    uint16_t wait;
    uint8_t junkRows = 0;
    uint8_t lineClears = 0;
    led_set(0, false);
    if (num_players == 2) {
        graphics_displayCharacter(' ');
        tinygl_update();
        while (1) {
            pacer_wait();
            if (ir_uart_read_ready_p()) {
                led_set(0, false);
                receivedChar = ir_uart_getc();
                if (receivedChar == 'r') {
                    ir_uart_putc ('r');
                    for (wait = 0; wait < 390; wait++) {
                        pacer_wait();
                    }
                    break;
                }
            } else {
                led_set(0, true);
                ir_uart_putc ('r');
                receivedChar = ir_uart_getc();
                if (receivedChar == 'r') {
                    break;
                }
            }
        }
    }
    led_set(0, false);
    tetris_init();
    uint8_t aTime = 35;
    Game *game = newGame();

    while (1) {

        for (wait = 0; wait < aTime; wait++) {
            pacer_wait();
            graphics_displayFramebuffer(game);

            button_update();
            navswitch_update ();
            if (button_down_p(0)) {
                aTime = 12;
            } else {
                aTime = 35;
            }
            if (check_move(game)) {
                graphics_fillFramebuffer(game);
            }
        }

        if (ir_uart_read_ready_p()) {
            receivedChar = ir_uart_getc();
            if (receivedChar == 'L') {
                destroyGame(game);
                if (num_players == 2){
                    return 1;
                } else {
                    return lineClears;
                }
            }
            if (receivedChar < 5 && receivedChar > 0) {
                junkRows += receivedChar;
                if(!physics_insertJunk(game, junkRows / 2)) {
                    ir_uart_putc ('L');
                    destroyGame(game);
                    if (num_players == 2){
                        return 0;
                    } else {
                        return lineClears;
                    }
                }
                junkRows = junkRows % 2;
            }
            
        }


        if (!physics_applyGravity(game)) {

            if (!physics_commitActiveTetrominoToStack(game)) {
                if (num_players == 2) {
                    ir_uart_putc ('L');
                }
                destroyGame(game);
                if (num_players == 2){
                    return 0;
                } else {
                    return lineClears;
                }
            }

            lineClears = physics_processLineClears(game);
            if (num_players == 2 && lineClears > 0) {
                ir_uart_putc (lineClears);
                lineClears = 0;
            }

            
            if (!spawnNextTetromino(game)) {
                if (num_players == 2) {
                    ir_uart_putc ('L');
                }
                destroyGame(game);
                return 0;
            }
            
        }
        
        graphics_fillFramebuffer(game);
    }
    
    destroyGame(game);

}

// returns true if we need to update the frame buffer
bool check_move(Game* game)
{
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        physics_moveActivePiece(game, RIGHT);

    } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        physics_moveActivePiece(game, LEFT);

    } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        physics_rotateActivePiece(game, COUNTERCLOCKWISE);

    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        physics_rotateActivePiece(game, CLOCKWISE);

    } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        holdPiece(game);

    } else {
        return false;
    }

    return true;
}