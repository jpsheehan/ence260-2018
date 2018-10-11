#include "showScreen.h"
#include "pio.h"
#include "../lib/utils/pacer.h"
#include "tetris.h"

/**
 * initialising columns
 */
uint8_t i;
uint8_t j;

void show_screen(uint8_t gameBoard[7][5]) {
    uint8_t cols[5] = {0};
    static uint8_t numberOfTimesCalled = 0;
    // iterate through columns
    for (i = 0; i < 5; i++) {
        //iterate through rows
        for (j = 0; j < 7; j++) {
            // If pixel is active on gameboard
            if (gameBoard[j][i] == STACK || gameBoard[j][i] == ACTIVE) {
                // if (gameBoard[j][i] == GHOST && numberOfTimesCalled % 2 == 0 || gameBoard[j][i] != GHOST) {
                    cols[i] |= (1 << j);
                // }
            }
        }
        ledmat_display_column (cols[i], i);
        pacer_wait();
    }
    pio_output_high(LEDMAT_COL5_PIO);
    numberOfTimesCalled++;
}

void screen_init() {
    ledmat_init();
}