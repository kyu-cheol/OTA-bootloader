#include <stdint.h>
#include "flash.h"

void flash_init(void)
{
    // 5 waits time, Data/Instruction cache enable, Prefetch enable
	FLASH->ACR |= 5 | FLASH_ACR_ENABLE_DATA_CACHE | FLASH_ACR_ENABLE_INST_CACHE | FLASH_ACR_ENABLE_PREFETCH;
}

void flash_lock(void)
{
    // flash lock 비트에 1 write
}

void flash_unlock(void)
{
    // flash lock 비트 확인

    // flash lock을 위한 key 값 write

    // flash lock 비트 해제까지 대기
}

void flash_erase(uint8_t sector_num)
{
    // flash의 busy 상태 확인

    // flash의 SER 비트를 켜서 sector 삭제 모드 활성화

    // flash의 STRT 비트를 켜서 sector 삭제 시작

    // flash의 busy 상태가 해제 될 때 까지 대기

    // 다음 작업을 위해 SER 비트 clear
}

void flash_write_word(uint32_t start_addr, uint32_t data)
{
    // flash의 busy 상태 확인

    // flash의 PG 비트를 켜서 프로그램 모드 활성화

    // flash 쓰기 단위 크기 설정

    // flash 주소에 data wrtie

    // flash의 busy 상태가 해제 될 때 까지 대기

    // 다음 작업을 위해 PG 비트 clear
}