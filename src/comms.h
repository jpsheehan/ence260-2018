/**
 * comms.h
 *
 * The comms module provides functions and macros for implementing IR communications between boards.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef COMMS_H_
#define COMMS_H_

#include <stdbool.h>
#include <stdint.h>

#include "tetris.h"

/**
 * Macros defining important signal codes.
 */
#define COMMS_LOSE_CHAR 'L'
#define COMMS_READY_CHAR 'r'

/**
 * Initialises the communications.
 */
void comms_init(void);

/**
 * Processes an incoming message.
 * An incoming message is either, some junk rows or notification that the other player has lost.
 *
 * @param game The game struct pointer.
 * @returns The state of the game, either GAME_NOT_OVER, GAME_OVER_LOSE or GAME_OVER_WIN
 */
uint8_t comms_processMessage(Game *game);

/**
 * Gets the status of the IR sensor.
 *
 * @returns true if there is a message waiting to be received.
 */
bool comms_readyToReceive(void);

/**
 * Sends the game over signal to the other board.
 */
void comms_sendGameOver(void);

/**
 * Sends notification of the number of lines that we have just cleared.
 *
 * @param num_line_clears The number of lines just cleared.
 */
void comms_sendLineClears(uint8_t num_line_clears);

/**
 * Waits for another player to be ready to play.
 */
void comms_waitForOtherPlayer(void);

#endif