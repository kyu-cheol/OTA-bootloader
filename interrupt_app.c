#include <stdint.h>
#include "timer.h"
#include "exti.h"

volatile uint32_t ticks;
uint8_t timer_flag;
uint8_t button_flag;

void isr_systick(void)
{
	ticks++;
}

void isr_tim2(void)
{
	static volatile uint32_t tim2_cnt = 0;

	TIM2_UI_FLAG_CLEAR();

	tim2_cnt++;
	timer_flag = 1;
}

void isr_exti4(void)
{
	EXTI->PR |= 1 << 4;
	button_flag = 1;
}
