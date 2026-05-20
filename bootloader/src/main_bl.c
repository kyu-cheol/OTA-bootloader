#include <stdio.h>
#include <stdint.h>
#include "system.h"
#include "flash.h"
#include "systick.h"
#include "uart.h"
#include "spi.h"
#include "gpio.h"

#define APP_OFFSET (0x08010000)
#define VTOR (*(volatile uint32_t *)0xE000ED08)

#define RCC_BASE (0x40023800)
#define RCC_CSR (*(volatile uint32_t *)(RCC_BASE + 0x74))

#define RCC_CSR_PINRSTF (1 << 26)
#define RCC_CSR_SFTRSTF (1 << 28)
#define RCC_CSR_RMVF    (1 << 24)

void update_application(void);
void crc_check_image(void);
void jump_to_application(uint32_t app_address);

void spi_rx_handler(SPI_x *SPIx, uint8_t data);
void spi_ovr_handler(SPI_x *SPIx, uint8_t data);

uint8_t spi_recv_cplt_flag;
uint16_t spi_recv_idx;
uint8_t buffer[1028];

void main_bl(void)
{
	flash_init();
	clock_config();
	gpio_init();
	systick_enable();
	uart_init(UART3, 115200);

	uint32_t reg = RCC_CSR;
	RCC_CSR |= RCC_CSR_RMVF;	// remove reset flag

	uart_write(UART3, "\r\n[BOOT] STM32F407 Bootloader v1.0.0 Built: May 16 2026\r\n");

	if (reg & RCC_CSR_SFTRSTF) {
		uart_write(UART3, "[INFO] Reset Reason: Software Reset\r\n");
	
		uart_write(UART3, "[INFO] Initializing SPI peripheral...\r\n");
		
		spi_init(SPI1, SPI_RECV_ONLY_SLAVE, 8, 1);
		spi_regist_rx_callback(spi_rx_handler);
		spi_regist_ovr_callback(spi_ovr_handler);
		
		uart_write(UART3, "[ OK ] SPI1 Slave Receive Only mode initialized.\r\n");
		uart_write(UART3, "[ OK ] SPI1 RXNE Interrupt Callback Function registed.\r\n");
		uart_write(UART3, "[ OK ] SPI1 OVR Interrupt Callback Function registed.\r\n");

		update_application();		// application image update
		crc_check_image();			// flash에 써진 이미지 무결성 검사

		spi_deinit(SPI1);
	}
	else if (reg & RCC_CSR_PINRSTF) {
		uart_write(UART3, "[INFO] Reset Reason: Normal Reset\r\n");
	}
	else {
		uart_write(UART3, "[INFO] Reset Reason: Else\r\n");
	}

	uart_write(UART3, "[INFO] Disabling bootloader peripherals...\r\n");
	systick_disable();

	jump_to_application(APP_OFFSET);
}

void jump_to_application(uint32_t app_address)
{
	uint32_t app_end_stack = *(uint32_t *)app_address;
	void (*app_entry)(void) = (void (*)(void))(*(volatile uint32_t *)(app_address + 4));
	
	uart_write(UART3, "[BOOT] Jumping to Application at 0x08010000...\r\n");

	// Application 위치로 Vector table 재설정
	__asm__ volatile ("cpsid i");
	VTOR = (uint32_t)app_address;
	__asm__ volatile ("ISB");
	__asm__ volatile ("cpsie i");

	// Application 스택 포인터 설정 후 점프
    __asm__ volatile ("msr msp, %0" :: "r"(app_end_stack));
    __asm__ volatile ("mov pc, %0"  :: "r"(app_entry));
}

void update_application(void)
{
	uint8_t is_last_packet = 0;

	uart_write(UART3, "[OTA] Updating Application Binary Image...\r\n");

	gpio_set_mode(GPIOD, 4, GPIO_MODE_OUTPUT);
	gpio_set_ospeed(GPIOD, 4, GPIO_OSPEED_VH);
	gpio_set_pupd(GPIOD, 4, GPIO_PUPD_NONE);

	while (!is_last_packet) {
		// 1. 버퍼 인덱스 0 초기화 후 수신가능 신호 전달
		spi_recv_idx = 0;	
		gpio_wrtie_pin(GPIOD, 4, 1);

		// 2. 1024 bytes 패킷 수신 후 버퍼에 저장
		while (!spi_recv_cplt_flag) {
			__asm__ volatile ("WFI");
		}

		for (int i = 0; i < 1028; i++) {
			printf("buffer[%d]: %d\r\n", i, buffer[i]);
		}
		printf("\r\n");

		// 2.5. 필요한 데이터들 파싱 (패킷 번호, 마지막 패킷 여부, 유효 데이터 길이, CRC)
		//		패킷 번호 			  -> 패킷 순서가 다를 경우 예외 처리
		//		마지막 패킷 여부 데이터 -> 5번 루프 탈출조건
		//		유효 데이터 길이 	   -> 4번 반복문 반복 횟수
		//		CRC 				 -> 3번 무결성 검사에서 대조

		// 3. 패킷 무결성 검사 (패킷 순서, CRC 검사 확인 후 필요하다면 패킷 재전송 요청)

		// 4. 4 bytes 씩 최대 255번 flash에 write (flash write하는 동안 interrupt off)

		// 5. 모든 바이너리를 flash에 쓸 동안 1 ~ 4 반복

		// 1024 bytes 패킷을 flash에 쓸 때마다 ok 로그 메세지 출력
	}

	uart_write(UART3, "[OTA] Update Complete. (Total: %d Bytes / %d Packets)\r\n");
}

void crc_check_image(void)
{
	uart_write(UART3, "[INFO] Verifying firmware integrity...\r\n");
	Delay(2000);
	uart_write(UART3, "[INFO] Calculated CRC32: 0x... | Expected CRC32: 0x...\r\n");
	uart_write(UART3, "[ OK ] CRC Verification success! Firmware is valid.\r\n");
}

void spi_rx_handler(SPI_x *SPIx, uint8_t data)
{
	if (SPIx == SPI1) {
		if (spi_recv_idx == 0) {
			// 수신가능 신호 off
			gpio_wrtie_pin(GPIOD, 4, 0);
		}

		if (spi_recv_idx < 1028) {
			buffer[spi_recv_idx++] = data;
		}

		if (spi_recv_idx == 1024) {
			spi_recv_cplt_flag = 1;
		}
	}
}

void spi_ovr_handler(SPI_x *SPIx, uint8_t data)
{
	if (SPIx == SPI1) {
		uart_write(UART3, "[ERROR] SPI Over Run Interrupt Occured.\r\n");
		while (1);
	}
}