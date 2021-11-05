#include "util.h"

volatile uint32_t tick = 0;

void incTick() {
    tick++;
}

uint32_t getTick() {
    return tick;
}