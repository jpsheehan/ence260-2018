#include "system.h"
#include "pacer.h"

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

    // init the IR comms
    
    // init the pacer (100Hz should be good enough to update the screen and the game)
    pacer_init(100);

    Game game = {0};
    spawnNextTetromino(&game);

    setState(STATE_STARTUP);

    while (1)
    {
        pacer_wait();
        
        fillFramebuffer(&game);
        show_screen(frameBuffer);

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
