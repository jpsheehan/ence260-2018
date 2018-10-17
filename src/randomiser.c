/**
 * randomiser.h
 *
 * The randomiser module provides functions and macros for generating random numbers and tetrominos.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

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

/**
 * The current bag.
 */
static uint8_t sevenBag[BAG_SIZE];

/**
 * The current position in the bag.
 */
static uint8_t sevenBagIndex;

/**
 * Gets whether or not the specific piece exists inside the current bag.
 * 
 * @param piece The piece to check for (O, I, T, S, Z, L or J)
 * @returns true if the piece exists in the bag.
 */
bool randomiser_existsInSevenBag(uint8_t piece)
{
    uint8_t i;
    for (i = 0; i < BAG_SIZE; i++)
    {
        if (sevenBag[i] == piece)
        {
            return true;
        }
    }
    return false;
}

/**
 * Destroys the current bag and creates a new one.
 */
void randomiser_generateSevenBag(void)
{
    // reset the contents of the bag
    memset(sevenBag, NONE, BAG_SIZE);

    uint8_t i;
    for (i = 0; i < BAG_SIZE; i++)
    {

        // pick a piece at random
        uint8_t piece = randomiser_getRand() % NUM_TETROMINOS;

        // if that piece already exists in the bag, draw a new piece until it doesn't exist
        // there are probably better ways of doing this
        while (randomiser_existsInSevenBag(piece))
        {
            piece = randomiser_getRand() % NUM_TETROMINOS;
        }

        sevenBag[i] = piece;
    }
    sevenBagIndex = 0;
}

/**
 * Gets the next piece in the bag. If the bag is empty it will generate a new bag behind the scenes.
 * 
 * @returns the next piece in the bag.
 */
uint8_t randomiser_getNextTetromino(void)
{
    if (sevenBagIndex == BAG_SIZE)
    {
        randomiser_generateSevenBag();
    }
    return sevenBag[sevenBagIndex++];
}

/**
 * Gets a general purpose 16-bit random number.
 * 
 * @returns The random number.
 */
uint16_t randomiser_getRand(void)
{
    return rand() + timer_get();
}