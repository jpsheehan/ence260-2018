#include <stdint.h>
#include <string.h>

#include "tetris.h"
#include "physics.h"

#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"
#include "../lib/utils/pacer.h"
#include "showScreen.h"
#include "ir_uart.h"

uint8_t FUNKY_WAIT_SCREEN[7][5] ={{0, 0, 0, 0, 0},
                            {0, 1, 1, 1, 0},
                            {1, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1},
                            {1, 0, 0, 0, 1},
                            {0, 1, 1, 1, 0},
                            {0, 0, 0, 0, 0}
                            };


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
        spawnTetromino(game, tempPiece);
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
    uint16_t wait;
    led_set(0, false);
    if (num_players == 2) {
        if (ir_uart_read_ready_p()) {
            led_set(0, true);
            ir_uart_getc();
            ir_uart_putc ('r');
            for (wait = 0; wait < 390; wait++) {
                pacer_wait();
            }
        } else {
            led_set(0, true);
            ir_uart_putc ('r');
            ir_uart_getc();
     
        }
    }
    led_set(0, false);
    tetris_init();
    uint8_t aTime = 35;
    uint8_t clears = 0;
    uint8_t junklines = 0;
    Game *game = newGame();

    while (1) {

        for (wait = 0; wait < aTime; wait++) {
            pacer_wait();
            show_screen(frameBuffer);
            

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
            ir_uart_getc();
            destroyGame(game);
            return 1;
        }


        if (!applyGravity(game)) {

            if (!commitActiveTetrominoToStack(game)) {
                if (num_players == 2) {
                    ir_uart_putc ('L');
                }
                destroyGame(game);
                return 0;
            }

            clears = processLineClears(game);

            if (num_players == 2 && clears > 0) {
                //TODO: send clears over IR if 2 player
                //ir_uart_putc (clears);
            }
            
            if (!spawnNextTetromino(game)) {
                if (num_players == 2) {
                    ir_uart_putc ('L');
                }
                destroyGame(game);
                return 0;
            }
            clears = 0;
            
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