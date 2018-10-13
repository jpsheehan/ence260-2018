#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"

#include "../lib/utils/pacer.h"

#include "state.h"
#include "tetris.h"
#include "menu.h"
#include "physics.h"
#include "graphics.h"
#include "ir_uart.h"
#include "../lib/utils/tinygl.h"

/**
 * Initialises the hardware and starts the main loop.
 */
int main (void)
{
    system_init ();

    ir_uart_init();

    // init the button
    button_init();

    // init the nav switch
    navswitch_init ();

    // init the led
    led_init();
    led_set(0, false);

    graphics_init();

    

    // init the IR comms
    
    // init the pacer (100Hz should be good enough to update the screen and the game)
    pacer_init(300);


    setState(STATE_STARTUP);

    
    uint8_t player_num = 1;
    uint8_t won;

    while (1)
    {
        pacer_wait();
        

        switch (getState()) {

            case STATE_STARTUP:
                tinygl_clear();
                while (1) {
                    displayCharacter('T');
                    navswitch_update();
                    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                        setState(STATE_MENU);
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                break;

            case STATE_MENU:
                tinygl_clear();
                while (1) {
                    navswitch_update();
                    if (player_num == 1) {
                        displayCharacter('1');
                    } else {
                        displayCharacter('2');
                    }

                    if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                        if (player_num == 1) {
                            player_num = 2;
                        } else {
                            player_num = 1;
                        }
                    }

                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        if (player_num == 1) {
                            won = playTetris(1);
                        } else {
                            won = playTetris(2);
                        }
                        if (won) {
                            setState(STATE_WON);
                        } else {
                            setState(STATE_LOST);
                        }
                        break;
                    }

                    tinygl_update();
                    pacer_wait();
                }
                break;

            case STATE_WON:
                tinygl_clear();
                while (1){
                    navswitch_update();
                    displayCharacter('W');
                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                setState(STATE_STARTUP);
                break;

            case STATE_LOST:
                tinygl_clear();
                while (1){
                    navswitch_update();
                    displayCharacter('L');
                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                setState(STATE_STARTUP);
                break;
        }


    }
}