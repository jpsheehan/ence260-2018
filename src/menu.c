/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#include "menu.h"
#include "state.h"
#include "tetris.h"
#include "graphics.h"

#include "navswitch.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "pacer.h"

/**
 * Displays the main menu to the user.
 */
void menu_main(void)
{

    uint8_t player_num = 1;
    
    uint8_t won = 0;
  pacer_wait();
        
        switch (state_get()) {

            case STATE_STARTUP:
                tinygl_clear();
                graphics_displayText("Tetris");
                while (1) {
                    navswitch_update();
                    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                        state_set(STATE_MENU);
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                break;

            case STATE_MENU:
                tinygl_clear();
                if (player_num == 1) {
                    graphics_displayText("1 Player");
                } else {
                    graphics_displayText("2 Player");
                }
                while (1) {
                    navswitch_update();

                    if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                        tinygl_clear();
                        if (player_num == 1) {
                            player_num = 2;
                            graphics_displayText("2 Player");
                        } else {
                            player_num = 1;
                            graphics_displayText("1 Player");
                        }
                    }

                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        tinygl_clear();
                        if (player_num == 1) {
                            won = playTetris(1);
                            // state_set(STATE_SCORE);
                            // break;
                        } else {
                            won = playTetris(2);
                        }
                        if (won) {
                            state_set(STATE_WON);
                        } else {
                            state_set(STATE_LOST);
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
                graphics_displayText("You Win!");
                while (1){
                    navswitch_update();
                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                state_set(STATE_STARTUP);
                break;

            case STATE_LOST:
                tinygl_clear();
                graphics_displayText("You Lose!");
                while (1){
                    navswitch_update();
                    if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                        break;
                    }
                    tinygl_update();
                    pacer_wait();
                }
                state_set(STATE_STARTUP);
                break;
        }

    }