#include <stdbool.h>
#include <stdint.h>

#include "led.h"
#include "ir_uart.h"
#include "pacer.h"

#include "tetris.h"
#include "comms.h"

static uint8_t junkRows;

void comms_init()
{
    // init the IR comms
    ir_uart_init();

    // init the led and turn it off
    led_init();
    led_set(0, false);
}

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

bool comms_readyToReceive()
{
    return ir_uart_read_ready_p();
}

void comms_sendGameOver()
{
    ir_uart_putc(COMMS_LOSE_CHAR);
}

void comms_sendLineClears(uint8_t num_line_clears)
{
    ir_uart_putc(num_line_clears);
}

void comms_waitForOtherPlayer()
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