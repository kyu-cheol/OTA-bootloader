#ifndef __RCC_H__
#define __RCC_H__

typedef struct RCC_TYPEDEF {
	uint32_t CR;
	uint32_t PLLCFGR;
	uint32_t CFGR;
} RCC_TypeDef;

#define RCC_BASE 0x40023800
#define RCC ((RCC_TypeDef *)RCC_BASE)

#define RCC_AHB1_CLOCK_RST (*(volatile uint32_t *)0x40023810)
#define RCC_APB1_CLOCK_RST (*(volatile uint32_t *)0x40023820)
#define RCC_APB2_CLOCK_RST (*(volatile uint32_t *)0x40023824)

#define RCC_AHB1_CLOCK_ER (*(volatile uint32_t *)0x40023830)
#define RCC_APB1_CLOCK_ER (*(volatile uint32_t *)0x40023840)
#define RCC_APB2_CLOCK_ER (*(volatile uint32_t *)0x40023844)

#define GPIOA_AHB1_CLOCK_ER_VAL (1 << 0)
#define GPIOB_AHB1_CLOCK_ER_VAL (1 << 1)
#define GPIOE_AHB1_CLOCK_ER_VAL (1 << 4)

#define TIM2_APB1_CLOCK_ER_VAL  (1 << 0)
#define UART3_APB1_CLOCK_ER_VAL (1 << 18)

#define SYSCFG_APB2_CLOCK_ER_VAL (1 << 14)

#endif
