#include "system.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"

/**
 * Initialises the hardware and starts the main loop.
 */
int main (void)
{
    system_init ();

    // init the led matrix
    //comment

    // init the button

    // init the IR comms
    
    // init the pacer (100Hz should be good enough to update the screen and the game)

    while (1)
    {
        switch (state) {
            case STARTUP:

                break;
            case MAIN_MENU:

                break;
            case SINGLE_PLAYER_GAME:
                playTetris(1);
                break;
            case MULTI_PLAYER_GAME:
                playTetris(2);
                break;
        }
    }
}
