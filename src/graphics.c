#include "pio.h"
#include "system.h"
#include "ledmat.h"
#include "display.h"

#include "../lib/utils/tinygl.h"
#include "../lib/fonts/font3x5_1.h"
#include "../lib/fonts/font5x7_1.h"
#include "../lib/utils/pacer.h"

#include "graphics.h"
#include "physics.h"
#include "tetris.h"

void graphics_init(void)
{

    // initialise TinyGL
    tinygl_init (300);
    tinygl_font_set (&font5x7_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_clear ();
    tinygl_text_speed_set (10);

    // initialise the LED matrix
    ledmat_init();
}

void displayCharacter(char c)
{
  char string[2];
  string[0] = c;
  string[1] = 0;

  tinygl_text(string);
}

void fillFramebuffer(Game *game)
{   
    // copy the stack data and clear the frameBuffer at the same time
    uint8_t i = 0;
    for (; i < GAME_BOARD_HEIGHT; i++) {
        uint8_t j = 0;
        for (; j < GAME_BOARD_WIDTH; j++) {
            if (game->board[i][j]) {
                game->framebuffer[i][j] = STACK;
            } else {
                game->framebuffer[i][j] = EMPTY;
            }
        }
    }

    // make it easier to access thhe position of the active piece
    Position* absPos = &game->active_position;
    Position *posData = getCollisionData(game->active_piece, game->active_orientation);

    // draw the active piece
    for (i = 0; i < 4; i++) {
        Position relPos = posData[i];
        int8_t x = absPos->x + relPos.x;
        int8_t y = absPos->y + relPos.y;

        if (x >= 0 && y >= 0 && x < GAME_BOARD_WIDTH && y < GAME_BOARD_HEIGHT && game->framebuffer[y][x] != STACK) {
            game->framebuffer[y][x] = ACTIVE;
        }
    }
    
}

/**
 * initialising columns
 */
void show_screen(uint8_t gameBoard[7][5]) {
    uint8_t cols[5] = {0};

    // iterate through columns
    uint8_t i;
    for (i = 0; i < 5; i++) {
        //iterate through rows
        uint8_t j;
        for (j = 0; j < 7; j++) {
            // If pixel is active on gameboard
            if (gameBoard[j][i]) {
                cols[i] |= (1 << j);
            }
        }
        ledmat_display_column (cols[i], i);
        pacer_wait();
    }
    pio_output_high(LEDMAT_COL5_PIO);
}