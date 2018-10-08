#include "timer.h"


uint8_t getPiece(void) {
    uint8_t i;
    i = timer_get (void) % 7;
    return i;
}