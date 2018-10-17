/**
 * tetris.c
 *
 * The tetris module provides functions for implementing the tetris game logic (except physics).
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include "pacer.h"
#include "timer.h"
#include "tinygl.h"

#include "tetris.h"
#include "physics.h"
#include "graphics.h"
#include "randomiser.h"
#include "comms.h"

/**
 * Spawns a new tetromino at the default location.
 * 
 * @param game The game struct pointer.
 * @param piece The piece to spawn (I, O, T, S, Z, L or J).
 */
void tetris_spawnTetromino(Game *game, Piece piece)
{
    game->piece = piece;
    game->position = (Position){DefaultSpawnPosition.x, DefaultSpawnPosition.y};
    game->orientation = ROTATE_0;
}

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false.
 * 
 * @param game The game struct pointer.
 * @returns true if the piece is able to spawn.
 */
bool tetris_spawnNextTetromino(Game *game)
{
    tetris_spawnTetromino(game, randomiser_getNextTetromino());
    game->has_held_this_turn = false;

    if (physics_testAbsolutePosition(game, game->position) == STACK)
    {
        tetris_commitActiveTetrominoToStack(game);
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * Attempts to hold the active piece. The player can only hold the piece once before they must lock down another piece.
 * 
 * @param game The game struct pointer.
 * @returns true if the piece is held
 */
bool tetris_holdPiece(Game *game)
{
    if (game->has_held_this_turn == false)
    {
        Piece tempPiece = game->held_piece;
        game->held_piece = game->piece;
        if (tempPiece == NONE)
        {
            tetris_spawnTetromino(game, randomiser_getNextTetromino());
        }
        else
        {
            tetris_spawnTetromino(game, tempPiece);
        }
        game->has_held_this_turn = true;
        return true;
    }
    else
    {
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
bool tetris_commitActiveTetrominoToStack(Game *game)
{
    bool isGameOver = false;
    uint8_t i;
    for (i = 0; i < NUM_MINOS_IN_PIECE; i++)
    {
        Position relPos = physics_getCollisionData(game->piece, game->orientation)[i];
        int8_t x = game->position.x + relPos.x;
        int8_t y = game->position.y + relPos.y;
        if (y < 0)
        {
            isGameOver = true;
        }
        else
        {
            game->board[y][x] = STACK;
        }
    }
    return !isGameOver;
}

/**
 * Checks each row in the game board for any line clears and increments the game score.
 * If any are found, they are removed and everything above is moved down.
 * 
 * @param game The game struct pointer.
 * @returns The number of lines cleared.
 */
uint8_t tetris_processLineClears(Game *game)
{
    uint8_t num_clears = 0;

    // loop through each row
    uint8_t i;
    for (i = 0; i < GAME_BOARD_HEIGHT; i++)
    {

        // loop through each cell, checking that it is full (a part of the stack)
        uint8_t j;
        for (j = 0; j < GAME_BOARD_WIDTH; j++)
        {

            if (game->board[i][j] != STACK)
            {
                break;
            }
        }

        // if this row is full
        if (j == GAME_BOARD_WIDTH)
        {
            num_clears++;

            // we must now clear this row by shifting all rows above it down by 1

            // starting at the current row (the row to be cleared) and working our way up to the top of the board,
            // we must shift each k-1 row to the kth row position
            uint8_t k;
            for (k = i; k > 0; k--)
            {

                // for each cell in the previous row move it down one
                uint8_t j;
                for (j = 0; j < GAME_BOARD_WIDTH; j++)
                {
                    game->board[k][j] = game->board[k - 1][j];
                }
            }

            // we must now clear the 0th row, because there cannot possibly be any stack there
            uint8_t j;
            for (j = 0; j < GAME_BOARD_WIDTH; j++)
            {
                game->board[0][j] = 0;
            }
        }
    }

    game->score += num_clears;

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
bool tetris_insertJunk(Game *game, uint8_t num_lines)
{
    // generate a line of junk to insert. a line of junk has one "hole" in it
    uint8_t holePosition = randomiser_getRand() % GAME_BOARD_WIDTH;
    bool is_game_lost = false;

    // push the active piece up as well,
    // we don't want to be dicks about the fact that you're losing
    game->position.y -= 1;

    // insert n lines of junk
    uint8_t i;
    for (i = 0; i < num_lines; i++)
    {

        // first of all, shift everything in the entire stack up one
        uint8_t j;
        for (j = 0; j < GAME_BOARD_HEIGHT - 1; j++)
        {

            // shift each row up one
            uint8_t k;
            for (k = 0; k < GAME_BOARD_WIDTH; k++)
            {

                // if this is the first row, we check to see if any stack would be pushed
                // up out of the playfield
                if (j == 0)
                {
                    if (game->board[j][k] == STACK)
                    {
                        is_game_lost = true;
                    }
                }

                game->board[j][k] = game->board[j + 1][k];
            }
        }

        // insert the junk
        for (j = 0; j < GAME_BOARD_WIDTH; j++)
        {
            if (j == holePosition)
            {
                game->board[GAME_BOARD_HEIGHT - 1][j] = EMPTY;
            }
            else
            {
                game->board[GAME_BOARD_HEIGHT - 1][j] = STACK;
            }
        }
    }

    return !is_game_lost;
}

/**
 * Initialises the tetris module.
 */
void tetris_init(void)
{
    DefaultSpawnPosition = (Position){1, 0};
    randomiser_generateSevenBag();
}

/**
 * Creates a new game of tetris on the heap.
 * Must be freed with tetris_destroyGame() later.
 * 
 * @returns A pointer to the new game.
 */
Game *tetris_newGame(void)
{
    // initialise a new game object
    Game *game = malloc(sizeof(Game));
    game->piece = I;
    game->orientation = ROTATE_0;
    game->position = DefaultSpawnPosition;
    game->score = 0;
    game->held_piece = NONE;
    game->has_held_this_turn = false;

    // clear the game board and framebuffer
    uint8_t i;
    for (i = 0; i < GAME_BOARD_HEIGHT; i++)
    {
        uint8_t j;
        for (j = 0; j < GAME_BOARD_WIDTH; j++)
        {
            game->board[i][j] = EMPTY;
            game->framebuffer[i][j] = EMPTY;
        }
    }

    // reset the piece generator, spawn a new piece and populate the framebuffer
    randomiser_generateSevenBag();
    tetris_spawnNextTetromino(game);
    graphics_fillFramebuffer(game);

    // return the pointer to the newly created game
    return game;
}

/**
 * Frees up the memory associated with the game.
 * This must be called at some point when the program is finished with the game.
 * 
 * @param game The game struct pointer.
 */
void tetris_destroyGame(Game *game)
{
    free(game);
}

/**
 * Checks the inputs and performs some functions depending on what was pressed.
 * 
 * @param game The game struct pointer.
 * @returns true if something occurred.
 */
bool tetris_checkMove(Game *game)
{
    if (navswitch_push_event_p(NAVSWITCH_EAST))
    {
        physics_moveActivePiece(game, RIGHT);
    }
    else if (navswitch_push_event_p(NAVSWITCH_WEST))
    {
        physics_moveActivePiece(game, LEFT);
    }
    else if (navswitch_push_event_p(NAVSWITCH_NORTH))
    {
        physics_rotateActivePiece(game, COUNTERCLOCKWISE);
    }
    else if (navswitch_push_event_p(NAVSWITCH_SOUTH))
    {
        physics_rotateActivePiece(game, CLOCKWISE);
    }
    else if (navswitch_push_event_p(NAVSWITCH_PUSH))
    {
        tetris_holdPiece(game);
    }
    else
    {
        return false;
    }

    return true;
}

/**
 * Starts a game of tetris.
 * 
 * @param num_players The number of players this game is for (1 or 2).
 * @returns the score if a 1-player game, true if won if 2-player game.
 */
uint8_t tetris_play(uint8_t num_players)
{
    uint8_t gravity = SLOW_GRAVITY;

    // wait for another player if we are playing a 2-player game
    if (num_players == 2)
    {
        // clear the screen
        graphics_displayCharacter(' ');
        tinygl_update();

        // wait for the second player to join
        comms_waitForOtherPlayer();
    }

    Game *game = tetris_newGame();

    while (true)
    {

        uint16_t wait;
        for (wait = 0; wait < gravity; wait++)
        {
            pacer_wait();
            graphics_displayFramebuffer(game);
            button_update();
            navswitch_update();

            if (button_down_p(0))
            {
                gravity = FAST_GRAVITY;
            }
            else
            {
                gravity = SLOW_GRAVITY;
            }

            // if something happened, update the framebuffer
            if (tetris_checkMove(game))
            {
                graphics_fillFramebuffer(game);
            }
        }

        // process any messages from the other player
        if (num_players == 2 && comms_readyToReceive())
        {
            uint8_t result = comms_processMessage(game);

            if (result != GAME_NOT_OVER)
            {
                return result;
            }
        }

        // if applying the gravity somehow fails:
        if (!physics_applyGravity(game))
        {

            // attempt to commit the active tetromino to the stack
            if (!tetris_commitActiveTetrominoToStack(game))
            {

                // a new tetromino cannot spawn as it is being blocked by the stack

                // we send the game over signal if we are playing 2-player
                if (num_players == 2)
                {
                    comms_sendGameOver();
                    return GAME_OVER_LOSE;
                }

                // otherwise we return the score
                uint8_t score = game->score;
                tetris_destroyGame(game);
                return score;
            }

            // if we clear some lines, send that information to the other player
            uint8_t lineClears = tetris_processLineClears(game);
            if (num_players == 2 && lineClears > 0)
            {
                comms_sendLineClears(lineClears);
            }

            // attempt to spawn the next tetromino
            if (!tetris_spawnNextTetromino(game))
            {

                // we can't spawn the next tetromino!!

                // send a game over signal if we are playing two player
                if (num_players == 2)
                {
                    comms_sendGameOver();
                    return GAME_OVER_LOSE;
                }

                // otherwise, return the score
                uint8_t score = game->score;
                tetris_destroyGame(game);
                return score;
            }
        }

        // fill the framebuffer
        graphics_fillFramebuffer(game);
    }
}