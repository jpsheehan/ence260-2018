/**
 * Tetris Project
 * By Ben Slattery and Jesse Sheehan 2018
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

/**
 * Displays the main menu to the user.
 */
void menu_main(void);

void menu_startup(void);

void menu_player_selection(void);

void menu_score(uint8_t score);

void menu_won(void);

void menu_lost(void);

#endif