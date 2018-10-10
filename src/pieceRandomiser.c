#include "pieceRandomiser.h"

uint8_t getPiece(void) {
    uint8_t i;
    i = timer_get() % 7;
    return i;
}