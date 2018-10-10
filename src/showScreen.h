#ifndef SHOWSCREEN_H_
#define SHOWSCREEN_H_

#include "system.h"
#include "ledmat.h"
#include "display.h"

/**
 * displays the screen based on a 7*5 array
 */
void show_screen(uint8_t gameBoard[7][5]);

/**
 * initialise the screen
 */
void screen_init(void);

#endif