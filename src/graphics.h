/**
 * graphics.h
 *
 * The graphics module provides functions for displaying things on the screen and manipulating the framebuffer.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <stdint.h>
#include "tetris.h"

/**
 * Displays a single character on the screen.
 *
 * @param character The character that you want to display.
 */
void graphics_displayCharacter(char character);

/**
 * Displays the framebuffer on the screen.
 *
 * @param game The game struct pointer.
 */
void graphics_displayFramebuffer(Game *game);

/**
 * Displays a string scrolling across the screen.
 *
 * @param string The string to display on the screen.
 */
void graphics_displayText(const char *string);

/**
 * Clears and fills the framebuffer from the current game data.
 *
 * @param game The game struct pointer.
 */
void graphics_fillFramebuffer(Game *game);

/**
 * Initialises the graphics module.
 */
void graphics_init(void);

#endif