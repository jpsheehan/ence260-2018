#ifndef COMMS_H_
#define COMMS_H_

#include <stdbool.h>
#include <stdint.h>

#include "tetris.h"

#define COMMS_LOSE_CHAR 'L'
#define COMMS_READY_CHAR 'r'

void comms_init(void);

uint8_t comms_processMessage(Game *game);

/**
 * Gets the status of the IR sensor.
 * 
 * @returns true if the sensor is ready to receive information.
 */
bool comms_readyToReceive(void);

void comms_sendGameOver(void);

void comms_sendLineClears(uint8_t num_line_clears);

void comms_waitForOtherPlayer(void);

#endif