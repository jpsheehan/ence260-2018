#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
#include "tetris.h"

void graphics_init(void);

void displayCharacter(char c);

void fillFramebuffer(Game *game);

#endif