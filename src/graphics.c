/**
 * graphics.c
 *
 * The graphics module provides functions for displaying things on the screen and manipulating the framebuffer.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#include "pio.h"
#include "system.h"
#include "ledmat.h"
#include "display.h"
#include "tinygl.h"
#include "font3x5_1.h"
#include "font5x7_1.h"
#include "pacer.h"

#include "graphics.h"
#include "physics.h"
#include "tetris.h"
#include "game.h"

/**
 * Displays a single character on the screen.
 * 
 * @param character The character that you want to display.
 */
void graphics_displayCharacter(char character)
{
    char string[2];
    string[0] = character;
    string[1] = 0;

    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_text(string);
}

/**
 * Displays the framebuffer on the screen.
 * 
 * @param game The game struct pointer.
 */
void graphics_displayFramebuffer(Game *game)
{
    uint8_t cols[5] = {0};

    // iterate through columns
    uint8_t i;
    for (i = 0; i < 5; i++)
    {

        //iterate through rows
        uint8_t j;
        for (j = 0; j < 7; j++)
        {

            // If pixel is active on gameboard
            if (game->framebuffer[j][i])
            {
                cols[i] |= (1 << j);
            }
        }

        ledmat_display_column(cols[i], i);
        pacer_wait();
    }

    pio_output_high(LEDMAT_COL5_PIO);
}

/**
 * Displays a string scrolling across the screen.
 * 
 * @param string The string to display on the screen.
 */
void graphics_displayText(const char *string)
{
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(string);
}

/**
 * Clears and fills the framebuffer from the current game data.
 * 
 * @param game The game struct pointer.
 */
void graphics_fillFramebuffer(Game *game)
{
    // copy the stack data and clear the frameBuffer at the same time
    uint8_t i;
    for (i = 0; i < GAME_BOARD_HEIGHT; i++)
    {

        uint8_t j;
        for (j = 0; j < GAME_BOARD_WIDTH; j++)
        {

            if (game->board[i][j])
            {
                game->framebuffer[i][j] = STACK;
            }
            else
            {
                game->framebuffer[i][j] = EMPTY;
            }
        }
    }

    // make it easier to access thhe position of the active piece
    Position *absPos = &game->position;
    Position *posData = physics_getCollisionData(game->piece, game->orientation);

    // draw the active piece
    for (i = 0; i < 4; i++)
    {
        Position relPos = posData[i];
        int8_t x = absPos->x + relPos.x;
        int8_t y = absPos->y + relPos.y;

        if (x >= 0 && y >= 0 && x < GAME_BOARD_WIDTH && y < GAME_BOARD_HEIGHT && game->framebuffer[y][x] != STACK)
        {
            game->framebuffer[y][x] = ACTIVE;
        }
    }
}

/**
 * Initialises the graphics module.
 */
void graphics_init(void)
{

    // initialise TinyGL
    tinygl_init(GAME_RATE);
    tinygl_font_set(&font5x7_1);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);
    tinygl_clear();
    tinygl_text_speed_set(20);

    // initialise the LED matrix
    ledmat_init();
}