#include <stdint.h>
#include "system.h"
#include "systick.h"
#include "uart.h"

#define APP_OFFSET (0x08001000)
#define VTOR (*(volatile uint32_t *)0xE000ED08)

#define RCC_BASE (0x40023800)
#define RCC_CSR (*(volatile uint32_t *)(RCC_BASE + 0x74))

#define RCC_CSR_PINRSTF (1 << 26)
#define RCC_CSR_SFTRSTF (1 << 28)
#define RCC_CSR_RMVF    (1 << 24)

void update_application(void);
void crc_check_image(void);

void main_bl(void)
{
	const uint32_t *app_IV = (const uint32_t *)APP_OFFSET;
	void (* app_entry)(void);
	uint32_t app_end_stack;

	flash_init();
	clock_config();
	systick_enable();
	uart_init(UART3, 115200);

	uint32_t reg = RCC_CSR;

	if (reg & RCC_CSR_SFTRSTF) {
		RCC_CSR |= RCC_CSR_RMVF;	// remove reset flag
	
		uart_write(UART3, "\r\n(Enter Bootloader for OTA)\r\n");	
		update_application();		// application image update
		crc_check_image();		// update된 application image 무결성 검사
	}
	else if (reg & RCC_CSR_PINRSTF) {
		RCC_CSR |= RCC_CSR_RMVF;        // remove reset flag
		
		uart_write(UART3, "\r\nnormal reset\r\n");
	}
	else {
		RCC_CSR |= RCC_CSR_RMVF;        // remove reset flag
		
		uart_write(UART3, "else\r\n");
	}

	// interrupt를 주기적으로 일으키거나
	// application에서 다른 setting으로 사용될 하드웨어들은 disable 된 후
	// application으로 jump됨을 보장해야 함.
	systick_disable();

	asm volatile ("cpsid i");
	VTOR = (uint32_t)app_IV;
	asm volatile ("ISB");
	asm volatile ("cpsie i"); 

	app_end_stack = (*(uint32_t *)APP_OFFSET);
	app_entry = (void (*)(void))(*(uint32_t *)(APP_OFFSET + 4));

	asm volatile ("msr msp, %0" ::"r"(app_end_stack));
	asm volatile ("mov pc, %0" ::"r"(app_entry));
}

void update_application(void)
{
	uart_write(UART3, "application firmware updating...\r\n");
        Delay(3000);
}

void crc_check_image(void)
{
	uart_write(UART3, "image binary crc checking...\r\n");
	Delay(2000);	
}
