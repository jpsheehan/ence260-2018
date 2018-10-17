/**
 * menu.h
 *
 * The menu module provides functions for displaying menus.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
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
 * Displays the "You Lost!" message (for two-player games).
 * Transitions to the main menu.
 */
void menu_lost(void)
{

    tinygl_clear();
    graphics_displayText("You Lose!");
    while (1)
    {
        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH))
        {
            break;
        }
        tinygl_update();
        pacer_wait();
    }
    state_set(STATE_STARTUP);
}

/**
 * Handles the running of all the menus.
 */
void menu_main(void)
{
    pacer_wait();

    switch (state_get())
    {

    case STATE_STARTUP:
        menu_startup();
        break;

    case STATE_MENU:
        menu_player_selection();
        break;

    // case STATE_SCORE:
    //     menu_score(0);
    //     break;

    case STATE_WON:
        menu_won();
        break;

    case STATE_LOST:
        menu_lost();
        break;
    }
}

/**
 * Displays the player selection screen to the user.
 * Begins a one-player or two-player game.
 */
void menu_player_selection(void)
{
    uint8_t player_num = 1;

    tinygl_clear();
    if (player_num == 1)
    {
        graphics_displayText("1 Player");
    }
    else
    {
        graphics_displayText("2 Player");
    }
    while (1)
    {
        navswitch_update();

        if (navswitch_push_event_p(NAVSWITCH_NORTH) || navswitch_push_event_p(NAVSWITCH_SOUTH))
        {
            tinygl_clear();
            if (player_num == 1)
            {
                player_num = 2;
                graphics_displayText("2 Player");
            }
            else
            {
                player_num = 1;
                graphics_displayText("1 Player");
            }
        }

        if (navswitch_push_event_p(NAVSWITCH_PUSH))
        {
            tinygl_clear();
            if (player_num == 1)
            {
                tetris_play(1);
                // state_set(STATE_SCORE);
                // break;
                state_set(STATE_LOST);
            }
            else
            {
                if (tetris_play(2) == GAME_OVER_WIN)
                {
                    state_set(STATE_WON);
                }
                else
                {
                    state_set(STATE_LOST);
                }
            }
            break;
        }

        tinygl_update();
        pacer_wait();
    }
}

/**
 * Displays the player's score.
 * Transitions to the main menu.
 *
 * @param score The score to be displayed.
 */
// void menu_score(uint8_t score)
// {

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
// }

/**
 * Displays the startup screen to the user.
 * Transitions to the player selection menu.
 */
void menu_startup(void)
{
    tinygl_clear();
    graphics_displayText("Tetris");
    while (1)
    {
        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH))
        {
            state_set(STATE_MENU);
            break;
        }
        tinygl_update();
        pacer_wait();
    }
}

/**
 * Displays the "You Won!" message (for two-player games).
 * Transitions to the main menu.
 */
void menu_won(void)
{

    tinygl_clear();
    graphics_displayText("You Win!");
    while (1)
    {
        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH))
        {
            break;
        }
        tinygl_update();
        pacer_wait();
    }
    state_set(STATE_STARTUP);
}