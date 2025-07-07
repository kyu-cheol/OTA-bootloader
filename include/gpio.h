#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

typedef struct GPIO_PORT {
	uint32_t MODE;
	uint32_t OTYPE;
	uint32_t OSPEED;
	uint32_t PUPD;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t LCKR;
	uint32_t AFRL;
	uint32_t AFRH;
} GPIO_Port;

#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOE_BASE 0x40021000

#define GPIOA ((GPIO_Port *)GPIOA_BASE)
#define GPIOB ((GPIO_Port *)GPIOB_BASE)
#define GPIOE ((GPIO_Port *)GPIOE_BASE)

//GPIO_Port *GPIOA =  (GPIO_Port *)GPIOA_BASE;
//GPIO_Port *GPIOB =  (GPIO_Port *)GPIOB_BASE

#define GPIO_MODE_INPUT  0x00
#define GPIO_MODE_OUTPUT 0x01
#define GPIO_MODE_AF     0x02
#define GPIO_MODE_ANALOG 0x03

#define GPIO_OSPEED_L  0x00
#define GPIO_OSPEED_M  0x01
#define GPIO_OSPEED_H  0x02
#define GPIO_OSPEED_VH 0x03

#define GPIO_PUPD_NONE 0x00
#define GPIO_PUPD_PU   0x01
#define GPIO_PUPD_PD   0x02

//void gpio_pin_setup(void);

#endif
