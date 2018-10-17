/**
 * menu.h
 *
 * The menu module provides functions for displaying menus.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

/**
 * Displays the "You Lost!" message (for two-player games).
 * Transitions to the main menu.
 */
void menu_lost(void);

/**
 * Handles the running of all the menus.
 */
void menu_main(void);

/**
 * Displays the player selection screen to the user.
 * Begins a one-player or two-player game.
 */
void menu_player_selection(void);

/**
 * Displays the player's score.
 * Transitions to the main menu.
 *
 * @param score The score to be displayed.
 */
// void menu_score(uint8_t score);

/**
 * Displays the startup screen to the user.
 * Transitions to the player selection menu.
 */
void menu_startup(void);

/**
 * Displays the "You Won!" message (for two-player games).
 * Transitions to the main menu.
 */
void menu_won(void);

#endif