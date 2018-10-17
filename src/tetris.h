/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
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
 * Represents the Tetrominos.
 * The origin of all pieces is defined as being the topmost cell. If there is
 * more than one topmost cell, then the topmost and leftmost cell is the origin
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
 * Some useful macros for the size of the game board
 */
#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 7
#define NUM_MINOS_IN_PIECE 4
#define NUM_TETROMINOS 7

/**
 * Some useful macros for specifying the type of block at a particular place
 */
#define EMPTY 0
#define WALL 1
#define STACK 2
#define ACTIVE 3
#define GHOST 4
#define FLOOR 5

/**
 * Related to random piece generation.
 */
#define BAG_SIZE 7

/**
 * Represents a 2D position.
 */
typedef struct position_s {
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
typedef struct game_s {
    uint8_t board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH]; // contains information about the stack ONLY
    uint8_t framebuffer[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH]; // contains the data to be written to the screen
    Piece active_piece;
    Orientation active_orientation;
    Position active_position;
    uint16_t score;
    Piece held_piece;
    bool has_held_this_turn;
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
bool physics_commitActiveTetrominoToStack(Game* game);

/**
 * Checks each row in the game board for any line clears.
 * If any are found, they are removed and everything above is moved down.
 * 
 * @param game The game struct pointer.
 * @returns The number of lines cleared.
 */
uint8_t physics_processLineClears(Game* game);

/**
 * Inserts n lines of junk at the bottom of the stack.
 * If this causes any part of the stack to collide with the "sky" this function returns false as the game is over.
 * To be fair to the player, the active piece is also moved up one piece.
 *
 * @param game The game struct pointer.
 * @param num_lines The number of lines of junk to insert.
 * @returns true if the game is not over.
 */
bool physics_insertJunk(Game* game, uint8_t num_lines);

/**
 * To be called to initialise the game.
 */
void tetris_init(void);

/**
 * Starts a game of tetris.
 */
uint8_t playTetris(uint8_t num_players);

/**
 * Generates the next (or first) tetromino and spawns it.
 */
bool spawnNextTetromino(Game* game);

/**
 * Attempts to hold the active piece.
 * Returns true if successful.
 */
bool holdPiece(Game* game);

bool check_move(Game* game);

Game* newGame(void);

#endif