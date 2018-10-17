/**
 * comms.c
 *
 * The comms module provides functions and macros for implementing IR communications between boards.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include <stdbool.h>
#include <stdint.h>

#include "led.h"
#include "ir_uart.h"
#include "pacer.h"

#include "tetris.h"
#include "comms.h"

/**
 * Stores the current amount of junk rows received this game.
 * This is reset when comms_waitForOtherPlayer is called.
 */
static uint8_t junkRows;

/**
 * Initialises the communications.
 */
void comms_init(void)
{
    // init the IR comms
    ir_uart_init();

    // init the led and turn it off
    led_init();
    led_set(0, false);
}

/**
 * Processes an incoming message.
 * An incoming message is either, some junk rows or notification that the other player has lost.
 *
 * @param game The game struct pointer.
 * @returns The state of the game, either GAME_NOT_OVER, GAME_OVER_LOSE or GAME_OVER_WIN
 */
uint8_t comms_processMessage(Game *game)
{
    char receivedChar = ir_uart_getc();

    if (receivedChar == COMMS_LOSE_CHAR)
    {
        tetris_destroyGame(game);
        return GAME_OVER_WIN;
    }

    if (receivedChar < 5 && receivedChar > 0)
    {
        junkRows = +receivedChar;
        if (!tetris_insertJunk(game, junkRows / 2))
        {
            tetris_destroyGame(game);
            comms_sendGameOver();
            return GAME_OVER_LOSE;
        }

        junkRows = junkRows % 2;
    }

    return GAME_NOT_OVER;
}

/**
 * Gets the status of the IR sensor.
 *
 * @returns true if there is a message waiting to be received.
 */
bool comms_readyToReceive(void)
{
    return ir_uart_read_ready_p();
}

/**
 * Sends the game over signal to the other board.
 */
void comms_sendGameOver(void)
{
    ir_uart_putc(COMMS_LOSE_CHAR);
}

/**
 * Sends notification of the number of lines that we have just cleared.
 *
 * @param num_line_clears The number of lines just cleared.
 */
void comms_sendLineClears(uint8_t num_line_clears)
{
    ir_uart_putc(num_line_clears);
}

/**
 * Waits for another player to be ready to play.
 */
void comms_waitForOtherPlayer(void)
{
    // reset the number of line clears received
    junkRows = 0;

    char receivedChar = 0;
    while (1)
    {
        pacer_wait();
        if (ir_uart_read_ready_p())
        {
            led_set(0, false);
            receivedChar = ir_uart_getc();
            if (receivedChar == COMMS_READY_CHAR)
            {
                ir_uart_putc(COMMS_READY_CHAR);
                uint16_t wait;
                for (wait = 0; wait < 390; wait++)
                {
                    pacer_wait();
                }
                break;
            }
        }
        else
        {
            led_set(0, true);
            ir_uart_putc(COMMS_READY_CHAR);
            receivedChar = ir_uart_getc();
            if (receivedChar == COMMS_READY_CHAR)
            {
                break;
            }
        }
    }

    led_set(0, false);
}