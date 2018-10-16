/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#include "system.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "pio.h"
#include <stdlib.h>
#include <string.h>

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
    uint8_t won = 0;

    while (1)
    {
        pacer_wait();
        

        switch (getState()) {

            case STATE_STARTUP:
                tinygl_clear();
                displayText("Tetris");
                while (1) {
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
                if (player_num == 1) {
                    displayText("1 Player");
                } else {
                    displayText("2 Player");
                }
                while (1) {
                    navswitch_update();

                    if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                        tinygl_clear();
                        if (player_num == 1) {
                            player_num = 2;
                            displayText("2 Player");
                        } else {
                            player_num = 1;
                            displayText("1 Player");
                        }
                    }

                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        tinygl_clear();
                        if (player_num == 1) {
                            won = playTetris(1);
                            // setState(STATE_SCORE);
                            // break;
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

            //  case STATE_SCORE:
            //     tinygl_clear();
            //     char buffer[20];
            //     strncat(buffer, "Score: ", 7);
            //     char score[5];
            //     itoa(score, won, 10);
            //     strcat(buffer, score);
            //     displayText(buffer);
            //     while (1){
            //         navswitch_update();
            //         if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            //             break;
            //         }
            //         tinygl_update();
            //         pacer_wait();
            //     }
            //      setState(STATE_STARTUP);
            //      break;

            case STATE_WON:
                tinygl_clear();
                displayText("You Win!");
                while (1){
                    navswitch_update();
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
                displayText("You Lose!");
                while (1){
                    navswitch_update();
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