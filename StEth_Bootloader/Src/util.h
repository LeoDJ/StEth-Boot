#include <stdint.h>
#include "main.h"

void incTick();
uint32_t getTick();
uint32_t DWT_Delay_Init(void);
void DWT_Delay_us(volatile uint32_t microseconds);