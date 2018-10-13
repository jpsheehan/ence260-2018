#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
#include "tetris.h"

void graphics_init(void);

void displayCharacter(char c);

void fillFramebuffer(Game *game);

/**
 * displays the screen based on a 7*5 array
 */
void show_screen(uint8_t gameBoard[7][5]);

#endif