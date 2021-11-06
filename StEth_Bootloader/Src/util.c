#include "util.h"

volatile uint32_t tick = 0;

// call in SysTick_Handler
void incTick() {
    tick++;
}

uint32_t getTick() {
    return tick;
}

/**
 * @brief  Initializes DWT_Clock_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 *         1: clock cycle counter not started
 *         0: clock cycle counter works
 */
uint32_t DWT_Delay_Init(void) {
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // 0x00000001;

    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;

    /* 3 NO OPERATION instructions */
    __ASM volatile("NOP");
    __ASM volatile("NOP");
    __ASM volatile("NOP");

    /* Check if clock cycle counter has started */
    if (DWT->CYCCNT) {
        return 0; /*clock cycle counter started*/
    } 
    else {
        return 1; /*clock cycle counter not started*/
    }
}

/**
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
void DWT_Delay_us(volatile uint32_t microseconds)
{
    extern uint32_t SystemCoreClock;
    uint32_t clk_cycle_start = DWT->CYCCNT;
    
    /* Go to number of cycles for system */
    microseconds *= (SystemCoreClock / 1000000);
    
    /* Delay till end */
    while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}