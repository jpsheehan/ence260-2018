
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "timer.h"

#include "randomiser.h"
#include "tetris.h"

/**
 * We generate random numbers using the 7-bag system.
 * Imagine all 7 different pieces are placed in a bag and drawn at random one-by-one.
 * When you run out of pieces in the bag you put all the pieces back and start again.
 */
static uint8_t sevenBag[BAG_SIZE];
static uint8_t sevenBagIndex;

bool randomiser_existsInSevenBag(uint8_t piece)
{
    uint8_t i;
    for (i = 0; i < BAG_SIZE; i++) {
        if (sevenBag[i] == piece) {
            return true;
        }
    }
    return false;
}

uint16_t randomiser_getRand(void) {
    return rand() + timer_get();
}

void randomiser_generateSevenBag(void)
{
    // reset the contents of the bag
    memset(sevenBag, NONE, BAG_SIZE);

    uint8_t i;
    for (i = 0; i < BAG_SIZE; i++) {

        // pick a piece at random
        uint8_t piece = randomiser_getRand() % NUM_TETROMINOS;

        while (randomiser_existsInSevenBag(piece)) {
            piece = randomiser_getRand() % NUM_TETROMINOS;
        }

        sevenBag[i] = piece;
    }
    sevenBagIndex = 0;
}

/**
 * Returns a number from 0 - 6 (inclusive) that represents the next tetromino to spawned
 */
uint8_t randomiser_getNextTetromino(void) {
    if (sevenBagIndex == BAG_SIZE) {
        randomiser_generateSevenBag();
    }
    return sevenBag[sevenBagIndex++];
}