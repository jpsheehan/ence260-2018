/**
 * game.c
 *
 * Contains the main function.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "pacer.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"
#include "physics.h"
#include "graphics.h"
#include "comms.h"
#include "game.h"

/**
 * The main function. This is called to start the program.
 * Initialises the modules.
 *
 * @returns The status code (unused on the funkit).
 */
int main(void)
{
    // init the system
    system_init();

    // init the button
    button_init();

    // init the nav switch
    navswitch_init();

    // init the graphics module
    graphics_init();

    // init the pacer
    pacer_init(GAME_RATE);

    // init the communications
    comms_init();

    // init the state
    state_set(STATE_STARTUP);

    // init the tetris game
    tetris_init();

    while (true)
    {
        menu_main();
    }
}