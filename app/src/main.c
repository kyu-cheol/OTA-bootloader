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

void main(void) {
	//flash_init();
	//clock_config();
	systick_enable();
	timer_init(TIM2, 4200 - 1, 10000 - 1);
	led_setup();
	button_setup();
	//uart_init(UART3, 115200);
	
	spi_init(SPI1, SPI_RECV_ONLY_SLAVE, 8);
	spi_regist_rx_callback(spi_rx_handler);
	spi_regist_ovr_callback(spi_ovr_handler);

	uart_write(UART3, "(Enter Application Firmware!)\r\n");
	printf("[TEST] hello world\n");

	timer_start_IT(TIM2);	// 0.5s timer

	while (1) {
		if (timer_flag) {
			timer_flag = 0;

			led_toggle();
		}

		if (button_flag) {
			button_flag = 0;
		
			SoftwareReset();	
		}

		// led_toggle();
		// Delay(2000);
	}
}

void spi_rx_handler(SPI_x *SPIx, uint8_t data)
{
	//char str[10] = { 0, };

	if (SPIx == SPI1) {
		// sprintf(str, "recv :0x%x\r\n", data);
		// uart_write(UART3, str);
		printf("recv :0x%x\r\n", data);
		led_toggle();
	}
}

void spi_ovr_handler(SPI_x *SPIx, uint8_t data)
{
	if (SPIx == SPI1) {
		uart_write(UART3, "[ERROR] SPI Over Run Interrupt Occured.\r\n");
	}
}