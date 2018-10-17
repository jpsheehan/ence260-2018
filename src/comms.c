#include <stdbool.h>
#include <stdint.h>

#include "led.h"
#include "ir_uart.h"
#include "pacer.h"

#include "comms.h"

void comms_init()
{
    // init the IR comms
    ir_uart_init();

    // init the led and turn it off
    led_init();
    led_set(0, false);
}

void comms_waitForOtherPlayer()
{
    char receivedChar = 0;
    while (1)
    {
        pacer_wait();
        if (ir_uart_read_ready_p())
        {
            led_set(0, false);
            receivedChar = ir_uart_getc();
            if (receivedChar == 'r')
            {
                ir_uart_putc('r');
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
            ir_uart_putc('r');
            receivedChar = ir_uart_getc();
            if (receivedChar == 'r')
            {
                break;
            }
        }
    }

    led_set(0, false);
}