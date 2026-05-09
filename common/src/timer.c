#include <stdint.h>
#include "timer.h"
#include "nvic.h"
#include "rcc.h"

#define TIM_DIER_UIE 	     (1 << 0)	// Update Interrupt Enable
#define TIM_SR_UIF           (1 << 0)	// Update Interrupt Flag
#define TIM_EGR_UG			 (1 << 0)	// Update Generation
#define TIM_CR1_COUNT_ENABLE (1 << 0)
#define TIM_CR1_ONE_PULSE    (1 << 3)

static void timer_reset(TIM_x *tim)
{
	if (tim == TIM2) {
		RCC_APB1_CLOCK_RST |= TIM2_APB1_CLOCK_ER_VAL;
		__asm__ volatile ("DMB");
		RCC_APB1_CLOCK_RST &= ~TIM2_APB1_CLOCK_ER_VAL;
	}
}

static void timer_enable_clock(TIM_x *tim)
{
	if (tim == TIM2) {
		timer_reset(tim);
		RCC_APB1_CLOCK_ER |= TIM2_APB1_CLOCK_ER_VAL;
	}

	__asm__ volatile ("DMB");
}

static void timer_enable_interrupt(TIM_x *tim)
{
	if (tim == TIM2) {
		nvic_irq_enable(NVIC_TIM2_IRQN);
    	nvic_irq_setprio(NVIC_TIM2_IRQN, 0);
	}

	tim->DIER |= TIM_DIER_UIE;
}

void timer_init(TIM_x *tim, uint16_t psc, uint32_t arr)
{
	timer_enable_clock(tim);

	tim->CR1 = 0;   // disable timer counter
	tim->PSC = psc;
	tim->ARR = arr;

	/* PSC, ARR 설정을 즉시 하드웨어에 반영 */
	tim->EGR |= TIM_EGR_UG;
	tim->SR &= ~TIM_SR_UIF;

    __asm__ volatile ("DMB");
}

void timer_start(TIM_x *tim)
{
	tim->CR1 |= TIM_CR1_COUNT_ENABLE;
	__asm__ volatile ("DMB");
}

void timer_start_IT(TIM_x *tim)
{
	timer_enable_interrupt(tim);

    tim->CR1 |= TIM_CR1_COUNT_ENABLE;
    __asm__ volatile ("DMB");
}

