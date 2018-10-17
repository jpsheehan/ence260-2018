#ifndef RANDOMISER_H_
#define RANDOMISER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Related to random piece generation.
 */
#define BAG_SIZE 7


bool randomiser_existsInSevenBag(uint8_t piece);

uint16_t randomiser_getRand(void);

void randomiser_generateSevenBag(void);

uint8_t randomiser_getNextTetromino(void);


#endif