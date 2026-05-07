#include <stdint.h>
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

void main(void) {
	//flash_init();
	//clock_config();
	systick_enable();
	timer_init();
	led_setup();
	button_setup();
	//uart_init(UART3, 115200);
	
	spi_init(SPI1, SPI_RECV_ONLY_SLAVE, 8);
	spi_set_rx_callback(spi_rx_handler);

	uart_write(UART3, "(Enter Application Firmware!)\r\n");

	//timer_start_IT(4200 - 1, 10000 - 1);	// 0.5s timer

	while(1) {
		// if (timer_flag) {
		// 	timer_flag = 0;

		// 	led_toggle();
		// }

		if (button_flag) {
			button_flag = 0;
		
			SoftwareReset();	
		}

		// led_toggle();
		// Delay(500);
	}
}

void spi_rx_handler(SPI_x *SPIx, uint8_t data)
{
	if (SPIx == SPI1) {
		led_toggle();
	}
}