#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

typedef struct TIM_X {
    uint32_t CR1;       // 0x00
    uint32_t CR2;       // 0x04
    uint32_t SMCR;      // 0x08
    uint32_t DIER;      // 0x0C
    uint32_t SR;        // 0x10
    uint32_t EGR;       // 0x14
    uint32_t CCMR1;     // 0x18
    uint32_t CCMR2;     // 0x1C
    uint32_t CCER;      // 0x20
    uint32_t CNT;       // 0x24
    uint32_t PSC;       // 0x28
    uint32_t ARR;       // 0x2C
    uint32_t Reserved1; // 0x30
    uint32_t CCR1;      // 0x34
    uint32_t CCR2;      // 0x38
    uint32_t CCR3;      // 0x3C
    uint32_t CCR4;      // 0x40
    uint32_t Reserved2; // 0x44
    uint32_t DCR;       // 0x48
    uint32_t DMAR;      // 0x4C
} TIM_x;

#define TIM2_BASE 0x40000000

#define TIM2 ((TIM_x *)TIM2_BASE)

#define TIM2_UI_FLAG_CLEAR() TIM2->SR &= ~(1 << 0)

void timer_init(TIM_x *tim, uint16_t psc, uint32_t arr);
void timer_start(TIM_x *tim);
void timer_start_IT(TIM_x *tim);

#endif
