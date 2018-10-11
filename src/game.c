#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"

#include "../lib/utils/pacer.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"
#include "showScreen.h"
#include "physics.h"
#include "graphics.h"

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
    pacer_init(300);


    setState(STATE_1P_GAME);

    
    uint8_t player_num = 1;

    while (1)
    {
        pacer_wait();
        

        switch (getState()) {
            case STATE_STARTUP:
                while (1) {
                    show_screen(INIT_SCREEN);
                    navswitch_update();
                    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                        setState(STATE_MENU);
                        break;
                    }
                }
                break;
            case STATE_MENU:
                while (1) {
                    navswitch_update();
                    if (player_num == 1) {
                        show_screen(ONE);
                    } else {
                        show_screen(TWO);
                    }

                    if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                        if (player_num == 1) {
                            player_num = 2;
                        } else {
                            player_num = 1;
                        }
                    }
                    
                    pacer_wait();

                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        if (player_num == 1) {
                            setState(STATE_1P_GAME);
                        } else {
                            setState(STATE_2P_GAME);
                        }
                        break;
                    }
                }
                setState(STATE_1P_GAME);
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