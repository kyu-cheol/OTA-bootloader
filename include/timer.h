#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#define TIM2_BASE (0x40000000)
#define TIM2_CR1 (*(volatile uint32_t *)(TIM2_BASE + 0x00))
#define TIM2_DIER (*(volatile uint32_t *)(TIM2_BASE + 0x0c))
#define TIM2_SR (*(volatile uint32_t *)(TIM2_BASE + 0x10))
#define TIM2_CNT (*(volatile uint32_t *)(TIM2_BASE + 0x24))
#define TIM2_PSC (*(volatile uint32_t *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t *)(TIM2_BASE + 0x2c))

#define TIM2_UI_FLAG_CLEAR() TIM2_SR &= ~(1 << 0)

void timer_init();
void timer_start_IT(uint16_t psc, uint32_t arr);

#endif
