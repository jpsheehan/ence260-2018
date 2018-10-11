#include "system.h"
#include "../lib/utils/pacer.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"

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
    button_init();

    // init the nav switch
    navswitch_init ();

    // init the led
    led_init();
    led_set(0, PIO_OUTPUT_LOW);
    

    // init the IR comms
    
    // init the pacer (100Hz should be good enough to update the screen and the game)
    pacer_init(500);


    setState(STATE_STARTUP);

    uint8_t init_screen[7][5] ={ {1, 1, 0, 0, 0},
                         {1, 1, 0, 0, 0},
                         {1, 1, 1, 1, 0},
                         {1, 1, 1, 1, 0},
                         {0, 0, 1, 1, 0},
                         {0, 0, 1, 1, 0},
                         {0, 0, 0, 0, 0}
                       };

    while (1)
    {
        pacer_wait();
        

        switch (getState()) {
            case STATE_STARTUP:
                while (1) {
                    show_screen(init_screen);
                    navswitch_update();
                    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                        setState(STATE_1P_GAME);
                        break;
                    }
                }
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
