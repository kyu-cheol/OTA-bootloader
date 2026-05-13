#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "system.h"
#include "systick.h"
#include "timer.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "spi.h"

extern uint32_t ticks;
extern uint8_t timer_flag;
extern uint8_t button_flag;

void spi_rx_handler(SPI_x *SPIx, uint8_t data);
void spi_ovr_handler(SPI_x *SPIx, uint8_t data);

uint8_t buffer[640];
uint8_t test_flag;

void main(void) {
	//flash_init();
	//clock_config();
	systick_enable();
	timer_init(TIM2, 4200 - 1, 1000 - 1);
	led_setup();
	button_setup();
	//uart_init(UART3, 115200);
	
	spi_init(SPI1, SPI_RECV_ONLY_SLAVE, 8);
	spi_regist_rx_callback(spi_rx_handler);
	spi_regist_ovr_callback(spi_ovr_handler);

	uart_write(UART3, "(Enter Application Firmware!)\r\n");
	printf("[TEST] hello world\n\r");

	//timer_start_IT(TIM2);	// 0.5s timer
	timer_start_PWM(TIM2, 2, GPIOA, 1, 500);
	

	while (1) {
		// if (timer_flag) {
		// 	timer_flag = 0;

		// 	led_toggle();
		// }

		if (button_flag) {
			button_flag = 0;
		
			SoftwareReset();	
		}

		if (test_flag) {
			test_flag = 0;

			for (int i = 0; i < 640; i++) {
				printf("buffer[%d]: 0x%x\r\n", i, buffer[i]);
			}
		}

		led_toggle();
		Delay(500);
	}
}

void spi_rx_handler(SPI_x *SPIx, uint8_t data)
{
	static uint16_t idx = 0;

	if (SPIx == SPI1) {
		//printf("recv :0x%x\r\n", data);

		if (idx == 640) test_flag = 1;
		
		if (idx < 640) {
			buffer[idx++] = data;
		}
			
		//led_toggle();
	}
}

void spi_ovr_handler(SPI_x *SPIx, uint8_t data)
{
	if (SPIx == SPI1) {
		uart_write(UART3, "[ERROR] SPI Over Run Interrupt Occured.\r\n");
		while (1);
	}
}