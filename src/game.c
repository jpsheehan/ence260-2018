#include "system.h"
#include "pacer.h"
#include "navswitch.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"
#include "showScreen.h"
#include "physics.h"

/**
 * Initialises the hardware and starts the main loop.
 */
int main (void)
{
    system_init ();

    // init the led matrix
    screen_init();

    // init the button
    tetris_init();
    navswitch_init ();

    // init the IR comms
    
    // init the pacer (100Hz should be good enough to update the screen and the game)
    pacer_init(500);


    setState(STATE_1P_GAME);

    while (1)
    {
        pacer_wait();
        

        switch (getState()) {
            case STATE_STARTUP:

                break;
            case STATE_MENU:

                break;
            case STATE_1P_GAME:
                playTetris(1);
                break;
            case STATE_2P_GAME:
                playTetris(2);
                break;
        }


    }
}
