#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "tetris.h"
#include "physics.h"
#include "graphics.h"

#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"
#include "../lib/utils/pacer.h"
#include "showScreen.h"
#include "ir_uart.h"
#include "timer.h"


/**
 * We generate random numbers using the 7-bag system.
 * Imagine all 7 different pieces are placed in a bag and drawn at random one-by-one.
 * When you run out of pieces in the bag you put all the pieces back and start again.
 */
static uint8_t sevenBag[BAG_SIZE];
static uint8_t sevenBagIndex;

bool existsInSevenBag(uint8_t piece)
{
    uint8_t i = 0;
    for (; i < BAG_SIZE; i++) {
        if (sevenBag[i] == piece) {
            return true;
        }
    }
    return false;
}

uint16_t getRand(void) {
    return rand() + timer_get();
}

void generateSevenBag(void)
{
    // reset the contents of the bag
    memset(sevenBag, NONE, BAG_SIZE);

    uint8_t i = 0;
    for (; i < BAG_SIZE; i++) {

        // pick a piece at random
        uint8_t piece = getRand() % NUM_TETROMINOS;

        while (existsInSevenBag(piece)) {
            piece = getRand() % NUM_TETROMINOS;
        }

        sevenBag[i] = piece;
    }
    sevenBagIndex = 0;
}

/**
 * Returns a number from 0 - 6 (inclusive) that represents the next tetromino to spawned
 */
uint8_t getNextTetromino(void) {
    if (sevenBagIndex == BAG_SIZE) {
        generateSevenBag();
    }
    return sevenBag[sevenBagIndex++];
}


/**
 * spawns a particular tetromino.
 */
void spawnTetromino(Game* game, Piece piece)
{
    game->active_piece = piece;
    game->active_position = (Position){ DefaultSpawnPosition.x, DefaultSpawnPosition.y };
    game->active_orientation = ROTATE_0;
}

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetromino can be spawned it returns true.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false
 */
bool spawnNextTetromino(Game* game) {

    spawnTetromino(game, getNextTetromino());
    game->has_held_this_turn = false;
    if (testAbsolutePosition(game, game->active_position) == STACK) {
        commitActiveTetrominoToStack(game);
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
        game->held_piece = game->active_piece;
        if (tempPiece == NONE) {
            spawnTetromino(game, getNextTetromino());
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
 * To be called to initialise the game.
 */
void tetris_init(void) {
    DefaultSpawnPosition = (Position){1, 0};
    generateSevenBag();
}


// Allocates a new game on the heap
Game* newGame(void)
{
    // initialise a new game object
    Game* game = malloc(sizeof(Game));
    game->active_piece = I;
    game->active_orientation = ROTATE_0;
    game->active_position = DefaultSpawnPosition;
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

    generateSevenBag();
    spawnNextTetromino(game);
    fillFramebuffer(game);
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
        while (1) {
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
            show_screen(game->framebuffer);

            button_update();
            navswitch_update ();
            if (button_down_p(0)) {
                aTime = 12;
            } else {
                aTime = 35;
            }
            if (check_move(game)) {
                fillFramebuffer(game);
            }
        }

        if (ir_uart_read_ready_p()) {
            receivedChar = ir_uart_getc();
            if (receivedChar == 'L') {
                destroyGame(game);
                return 1;
            }
            if (receivedChar < 5 && receivedChar > 0) {
                junkRows += receivedChar;
                if(!insertJunk(game, junkRows / 2)) {
                    ir_uart_putc ('L');
                    destroyGame(game);
                    return 0;
                }
                junkRows = junkRows % 2;
            }
            
        }


        if (!applyGravity(game)) {

            if (!commitActiveTetrominoToStack(game)) {
                if (num_players == 2) {
                    ir_uart_putc ('L');
                }
                destroyGame(game);
                return 0;
            }

            lineClears = processLineClears(game);
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
        
        fillFramebuffer(game);
    }
    
    destroyGame(game);

}

// returns true if we need to update the frame buffer
bool check_move(Game* game)
{
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        moveActivePiece(game, RIGHT);

    } else if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        moveActivePiece(game, LEFT);

    } else if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        rotateActivePiece(game, COUNTERCLOCKWISE);

    } else if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        rotateActivePiece(game, CLOCKWISE);

    } else if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        holdPiece(game);

    } else {
        return false;
    }

    return true;
}