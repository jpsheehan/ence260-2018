#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdint.h>

/**
 * Represents the Tetrominos
 */
#define I 0
#define T 1
#define O 2
#define S 3
#define Z 4
#define L 5
#define J 6

/**
 * Represents the different states of rotation that the Tetrominos can be in.
 */
#define ROTATE_0 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3

/**
 * Some useful macros for the size of the game board
 */
#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 12
#define GAME_BOARD_VISIBLE_HEIGHT 7

/**
 * Represents a 2D position.
 */
typedef struct position_s {
    uint8_t x;
    uint8_t y;
} Position;

/**
 * Represents the entire state of the game.
 */
typedef struct game_s {
    uint8_t board[GAME_BOARD_HEIGHT][GAME_BOARD_WIDTH];
    uint8_t active_piece;
    uint8_t active_orientation;
    Position active_position;
    uint16_t score;
} Game;

/**
 * Starts a game of tetris.
 */
void playTetris(uint8_t num_players);

#endif