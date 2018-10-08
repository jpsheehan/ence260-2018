#include "ledmat.h"


/**
 * initialising columns
 */
uint8_t cols[5];

void show_screen(uint8_t gameBoard[7][5]) {
    // iterate through columns
    for (int i = 0; i < 5; i++) {
        //iterate through rows
        for (int j = 0; j < 7; j++) {
            // If pixel is active on gameboard
            if (gameBoard[j][i]) {
                cols[i] |= (1 << j)
            }
        }
        ledmad_display_column (cols[i], i);
    }
}