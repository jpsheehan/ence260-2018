/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"
#include "pacer.h"
#include "ir_uart.h"
#include "tinygl.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"
#include "physics.h"
#include "graphics.h"

/**
 * Initialises the hardware and starts the main loop.
 */
int main (void)
{
    system_init ();

    // init the IR comms
    ir_uart_init();

    // init the button
    button_init();

    // init the nav switch
    navswitch_init ();

    // init the led
    led_init();
    led_set(0, false);

    // init the graphics module
    graphics_init();
    
    // init the pacer (100Hz should be good enough to update the screen and the game)
    pacer_init(300);

    state_set(STATE_STARTUP);

    while (true)
    {
        menu_main();
    }
}