#include "gpio.h"
#include "rcc.h"

void gpio_set_mode(GPIO_Port *gpiox, uint8_t pin, uint8_t mode)
{
    uint32_t reg = gpiox->MODE;
    reg &= ~(0x03 << (pin * 2));
    reg |= (mode << (pin * 2));
    gpiox->MODE = reg;
}

void gpio_set_ospeed(GPIO_Port *gpiox, uint8_t pin, uint8_t ospeed)
{
    uint32_t reg = gpiox->OSPEED;
    reg &= ~(0x03 << (pin * 2));
    reg |= (ospeed << (pin * 2));
    gpiox->OSPEED = reg;
}

void gpio_set_pupd(GPIO_Port *gpiox, uint8_t pin, uint8_t pupd)
{
    uint32_t reg = gpiox->PUPD;
    reg &= ~(0x03 << (pin * 2));
    reg |= (pupd << (pin * 2));
    gpiox->PUPD = reg;
}

void gpio_set_af(GPIO_Port *gpiox, uint8_t pin, uint8_t af_num)
{
    uint32_t reg;

    if (pin < 8) {
        reg = gpiox->AFRL & (~(0x0f << (pin * 4)));
        gpiox->AFRL = reg | (af_num << (pin * 4));
    }
    else {
        reg = gpiox->AFRH & (~(0x0f << ((pin - 8) * 4)));
        gpiox->AFRH = reg | (af_num << ((pin - 8) * 4));
    }
}

void gpio_enable_clock(GPIO_Port *gpiox)
{
    if (gpiox == GPIOA) RCC_AHB1_CLOCK_ER |= GPIOA_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOB) RCC_AHB1_CLOCK_ER |= GPIOB_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOC) RCC_AHB1_CLOCK_ER |= GPIOC_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOD) RCC_AHB1_CLOCK_ER |= GPIOD_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOE) RCC_AHB1_CLOCK_ER |= GPIOE_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOF) RCC_AHB1_CLOCK_ER |= GPIOF_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOG) RCC_AHB1_CLOCK_ER |= GPIOG_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOH) RCC_AHB1_CLOCK_ER |= GPIOH_AHB1_CLOCK_ER_VAL;
	else if (gpiox == GPIOI) RCC_AHB1_CLOCK_ER |= GPIOI_AHB1_CLOCK_ER_VAL;
}