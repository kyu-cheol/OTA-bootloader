#include <stdint.h>
#include "system.h"
#include "systick.h"
#include "rcc.h"

#define AIRCR (*(volatile uint32_t *)0xE000ED0C)

/*** FLASH ***/
#define FLASH_BASE (0x40023C00)
#define FLASH_ACR  (*(volatile uint32_t *)(FLASH_BASE + 0x00))
#define FLASH_ACR_ENABLE_DATA_CACHE (1 << 10)
#define FLASH_ACR_ENABLE_INST_CACHE (1 << 9)
#define FLASH_ACR_ENABLE_PREFETCH   (1 << 8)

/*** RCC ***/
//#define RCC_BASE (0x40023800)
//#define RCC_CR      (*(volatile uint32_t *)(RCC_BASE + 0x00))
//#define RCC_PLLCFGR (*(volatile uint32_t *)(RCC_BASE + 0x04))
//#define RCC_CFGR    (*(volatile uint32_t *)(RCC_BASE + 0x08))

#define RCC_CR_PLLRDY               (1 << 25)
#define RCC_CR_PLLON                (1 << 24)
#define RCC_CR_HSERDY               (1 << 17)
#define RCC_CR_HSEON                (1 << 16)
#define RCC_CR_HSIRDY               (1 << 1)
#define RCC_CR_HSION                (1 << 0)

#define RCC_CFGR_SW_HSI             0x0
#define RCC_CFGR_SW_HSE             0x1
#define RCC_CFGR_SW_PLL             0x2

#define RCC_PLLCFGR_PLLSRC_HSE       (1 << 22)	// select HSE

#define PRESCALER_SYSCLK_TO_AHB_NONE 0
#define PRESCALER_SYSCLK_TO_AHB_2    8
#define PRESCALER_SYSCLK_TO_AHB_4    9

#define PRESCALER_AHB_TO_APB_NONE    0
#define PRESCALER_AHB_TO_APB_2	     4
#define PRESCALER_AHB_TO_APB_4	     5

#define PLL_FULL_MASK (0x0F437FFF)
#define PLLM 8
#define PLLN 336
#define PLLP 2
#define PLLQ 7
#define PLLR 0

void flash_init(void)
{
	FLASH_ACR |= 5 | FLASH_ACR_ENABLE_DATA_CACHE | FLASH_ACR_ENABLE_INST_CACHE | FLASH_ACR_ENABLE_PREFETCH;
}

void clock_config(void)
{
	uint32_t reg32;
	
	// HSI On
	RCC->CR |= RCC_CR_HSION;
	DMB();
	while ((RCC->CR & RCC_CR_HSIRDY) == 0);

	// Select SYSCLK with HSI
	reg32 = RCC->CFGR;
	reg32 &= ~((1 << 1) | (1 << 0));
	RCC->CFGR = (reg32 | RCC_CFGR_SW_HSI);
	DMB();

	// HSE On
	RCC->CR |= RCC_CR_HSEON;
	DMB();
	while ((RCC->CR & RCC_CR_HSERDY) == 0);

	// SYSCLK to AHB clock prescale
	reg32 = RCC->CFGR;
	reg32 &= ~0xF0;
	RCC->CFGR = (reg32 | (PRESCALER_SYSCLK_TO_AHB_NONE << 4));
	DMB();

	// AHB to APB1 clock prescale (1/4)
	reg32 = RCC->CFGR;
	reg32 &= ~0x1C00;	// PPRE1 clear
	RCC->CFGR = (reg32 | (PRESCALER_AHB_TO_APB_4 << 10));
	DMB();

	// AHB to APB2 clock prescale (1/2)
	reg32 = RCC->CFGR;
	reg32 &= ~(0x07 << 13);	// PPRE2 clear
	RCC->CFGR = (reg32 | (PRESCALER_AHB_TO_APB_2 << 13));
	DMB();

	// PLL config
	// PLL_OUT_FREQ = PLL_SRC_FREQ(HSE) / M * N / P
	reg32 = RCC->PLLCFGR;
	reg32 &= ~PLL_FULL_MASK;
	RCC->PLLCFGR = reg32 | RCC_PLLCFGR_PLLSRC_HSE | PLLM | (PLLN << 6) | (((PLLP >> 1) -1) << 16) | (PLLQ << 24);
	DMB();

	// PLL On
	RCC->CR |= RCC_CR_PLLON;
	DMB();
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);

	// Select SYSCLK with PLL
	reg32 = RCC->CFGR;
	reg32 &= ~((1 << 1) | (1 << 0));
	RCC->CFGR = (reg32 | RCC_CFGR_SW_PLL);
	DMB();
	while ((RCC->CFGR & ((1 << 1) | (1 << 0))) != RCC_CFGR_SW_PLL);

	// HSI Off
        RCC->CR &= ~RCC_CR_HSION;
        DMB();
}

void Delay(uint32_t delay)
{
	uint32_t tick_start = get_tick();
	uint32_t wait = delay;

	if (wait < MAX_DELAY) {
		wait += 1;
	}

	while ((get_tick() - tick_start) < wait);
}

void SoftwareReset(void)
{
	AIRCR = (0x05fa << 16) | (1 << 2);
	for (;;) {
		// wait until reset
	}
}
