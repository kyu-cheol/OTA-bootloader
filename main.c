#include <stdint.h>
#include <stdlib.h>
#include "gpio.h"
#include "system.h"
#include "systick.h"
#include "timer.h"
#include "led.h"
#include "uart.h"
#include "button.h"

extern uint32_t ticks;
extern uint8_t timer_flag;
extern uint8_t button_flag;

void main(void) {
	//flash_init();
	//clock_config();
	systick_enable();
	timer_init();
	led_setup();
	button_setup();
	//uart_init(UART3, 115200);

	uart_write(UART3, "(Enter Application Firmware!)\r\n");

	timer_start_IT(4200 - 1, 10000 - 1);	// 0.5s timer

	while(1) {
		/*		
		if (get_button_state()) {
			led_on();
		} else {
			led_off();
		}
		*/
		if (timer_flag) {
			timer_flag = 0;

			led_toggle();
		}

		if (button_flag) {
			button_flag = 0;
		
			SoftwareReset();	
		}
	}
}
