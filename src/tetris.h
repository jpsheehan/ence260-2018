/**
 * tetris.h
 *
 * The tetris module provides functions for implementing the tetris game logic (except physics).
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef TETRIS_H_
#define TETRIS_H_

/**
 * Platform independent:
 * Required for random number generation for spawnNextTetromino().
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * Represents each of the tetrominos.
 */
#define I 0
#define J 1
#define L 2
#define O 3
#define S 4
#define T 5
#define Z 6
#define NONE 255

/**
 * Macros for the size of the game board.
 */
#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 7

/**
 * Macro for specifying the number of squares in each piece.
 */
#define NUM_MINOS_IN_PIECE 4

/**
 * Macro for specifying the number of unique tetrominos.
 */
#define NUM_TETROMINOS 7

/**
 * Macros for specifying the type of block at a particular place.
 */
#define EMPTY 0
#define WALL 1
#define STACK 2
#define ACTIVE 3
#define GHOST 4
#define FLOOR 5

/**
 * Macros for defining the state of the game.
 */
#define GAME_OVER_LOSE 0
#define GAME_OVER_WIN 1
#define GAME_NOT_OVER 2

/**
 * Constants for slowing down or speeding up gravity.
 */
#define SLOW_GRAVITY 35;
#define FAST_GRAVITY 12;

/**
 * Represents a 2D position.
 */
typedef struct position_s
{
    int8_t x;
    int8_t y;
} Position;

/**
 * Represents one of ROTATE_0, ROTATE_90, ROTATE_180 or ROTATE_270.
 */
typedef uint8_t Orientation;

/**
 * Represents one of I, O, T, S, Z, L or J.
 */
typedef uint8_t Piece;

/**
 * Represents the entire state of the game.
 */
typedef struct game_s
{
    uint8_t board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];       // contains information about the stack ONLY.
    uint8_t framebuffer[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH]; // contains the data to be written to the screen.
    Piece piece;                                              // the currently active piece.
    Orientation orientation;                                  // the orientation of the currently active piece.
    Position position;                                        // the position of the currently active piece.
    uint16_t score;                                           // the score of this game.
    Piece held_piece;                                         // the piece that is currently being held. Defaults to NONE.
    bool has_held_this_turn;                                  // true if a piece has been held since the previous piece was locked down.
} Game;

/**
 * The default spawn position of all pieces.
 * This is set in the tetris_init() function.
 */
Position DefaultSpawnPosition;

/**
 * Commits the current active tetromino to the stack.
 * After commiting the active piece, it checks to make sure that the game is not over.
 *
 * @param game The game struct pointer.
 * @returns true if the game is not over.
 */
bool tetris_commitActiveTetrominoToStack(Game *game);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 *
 * @param game The game struct pointer.
 * @returns The number of lines cleared.
 */
uint8_t tetris_processLineClears(Game *game);

/**
 * Inserts n lines of junk at the bottom of the stack.
 * If this causes any part of the stack to collide with the "sky" this function returns false as the game is over.
 * To be fair to the player, the active piece is also moved up one piece.
 *
 * @param game The game struct pointer.
 * @param num_lines The number of lines of junk to insert.
 * @returns true if the game is not over.
 */
bool tetris_insertJunk(Game *game, uint8_t num_lines);

/**
 * To be called to initialise the game.
 */
void tetris_init(void);

/**
 * Starts a game of tetris.
 *
 * @param num_players The number of players this game is for (1 or 2).
 * @returns the score if a 1-player game, true if won if 2-player game.
 */
uint8_t tetris_play(uint8_t num_players);

/**
 * Generates the next (or first) tetromino and spawns it.
 * If a new tetronmino cannot be spawned it will turn the current tetromino into stack and return false.
 * The game is over if this function returns false.
 *
 * @param game The game struct pointer.
 * @returns true if the piece is able to spawn.
 */
bool tetris_spawnNextTetromino(Game *game);

/**
 * Attempts to hold the active piece. The player can only hold the piece once before they must lock down another piece.
 *
 * @param game The game struct pointer.
 * @returns true if the piece is held
 */
bool tetris_holdPiece(Game *game);

/**
 * Checks the inputs and performs some functions depending on what was pressed.
 *
 * @param game The game struct pointer.
 * @returns true if something occurred.
 */
bool tetris_checkMove(Game *game);

/**
 * Creates a new game of tetris on the heap.
 * Must be freed with tetris_destroyGame() later.
 *
 * @returns A pointer to the new game.
 */
Game *tetris_newGame(void);

/**
 * Frees up the memory associated with the game.
 * This must be called at some point when the program is finished with the game.
 *
 * @param game The game struct pointer.
 */
void tetris_destroyGame(Game *game);

/**
 * Spawns a new tetromino at the default location.
 *
 * @param game The game struct pointer.
 * @param piece The piece to spawn (I, O, T, S, Z, L or J).
 */
void tetris_spawnTetromino(Game *game, Piece piece);

#endif