#include <stdint.h>
#include "timer.h"
#include "nvic.h"
#include "rcc.h"

#define TIM_DIER_UIE 	     (1 << 0)	// Update Interrupt Enable
#define TIM_SR_UIF           (1 << 0)	// Update Interrupt Flag
#define TIM_CR1_COUNT_ENABLE (1 << 0)
#define TIM_CR1_ONE_PULSE    (1 << 3)

void timer_init()
{
	RCC_APB1_CLOCK_RST |= TIM2_APB1_CLOCK_ER_VAL;
	__asm__ volatile ("DMB");
	RCC_APB1_CLOCK_RST &= ~TIM2_APB1_CLOCK_ER_VAL;
	
	RCC_APB1_CLOCK_ER |= TIM2_APB1_CLOCK_ER_VAL;

	TIM2_CR1 = 0;   // disable timer counter
        __asm__ volatile ("DMB");

}

void timer_start_IT(uint16_t psc, uint32_t arr)
{
	nvic_irq_enable(NVIC_TIM2_IRQN);
        nvic_irq_setprio(NVIC_TIM2_IRQN, 0);

        TIM2_PSC = psc;
        TIM2_ARR = arr;

        TIM2_CR1 |= TIM_CR1_COUNT_ENABLE;
        TIM2_DIER |= TIM_DIER_UIE;
        __asm__ volatile ("DMB");
}
