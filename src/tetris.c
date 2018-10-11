#include <stdint.h>
#include <string.h>

#include "tetris.h"
#include "physics.h"

#ifdef __AVR__
    #include "navswitch.h"
    #include "button.h"
    #include "led.h"
    #include "pio.h"
    #include "../lib/utils/pacer.h"
    #include "showScreen.h"
#endif

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

void generateSevenBag(void)
{
    // reset the contents of the bag
    memset(sevenBag, NONE, BAG_SIZE);

    uint8_t i = 0;
    for (; i < BAG_SIZE; i++) {

        // pick a piece at random
        uint8_t piece = rand() % NUM_TETROMINOS;

        while (existsInSevenBag(piece)) {
            piece = rand() % NUM_TETROMINOS;
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


Game newGame(void)
{
    Game game = {{
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}}, I, ROTATE_0, DefaultSpawnPosition, 0, NONE, false };
    generateSevenBag();
    spawnNextTetromino(&game);
    return game;
}

#ifdef __AVR__
/**
 * Starts a game of tetris.
 */
void playTetris(uint8_t num_players)
{
    tetris_init();
    uint16_t wait;
    uint8_t aTime = 35;
    uint8_t clears = 0;
    Game game = newGame();

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
            if (check_move(&game)) {
                fillFramebuffer(&game);
            }
        }


        if (!applyGravity(&game)) {

            if (!commitActiveTetrominoToStack(&game)) {
                break;
            }

            clears = processLineClears(&game);

            if (num_players == 2 && clears > 0) {
                //TODO: send clears over IR if 2 player
            }
            
            if (!spawnNextTetromino(&game)) {
                break;
            }
            clears = 0;
            
        }
        
        fillFramebuffer(&game);
    }
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
#endif