/**
 * randomiser.h
 *
 * The randomiser module provides functions and macros for generating random numbers and tetrominos.
 *
 * ENCE260 Assignment
 * Written by Ben Slattery and Jesse Sheehan
 * October 2018
 */

#ifndef RANDOMISER_H_
#define RANDOMISER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * The size of the bag defines how many pieces can fit in the bag at any one time.
 */
#define BAG_SIZE 7

/**
 * Gets whether or not the specific piece exists inside the current bag.
 * 
 * @param piece The piece to check for (O, I, T, S, Z, L or J)
 * @returns true if the piece exists in the bag.
 */
bool randomiser_existsInSevenBag(uint8_t piece);

/**
 * Destroys the current bag and creates a new one.
 */
void randomiser_generateSevenBag(void);

/**
 * Gets the next piece in the bag. If the bag is empty it will generate a new bag behind the scenes.
 * 
 * @returns the next piece in the bag.
 */
uint8_t randomiser_getNextTetromino(void);

/**
 * Gets a general purpose 16-bit random number.
 * 
 * @returns The random number.
 */
uint16_t randomiser_getRand(void);

#endif