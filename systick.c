#include "systick.h"
#include "system.h"

void systick_enable(void)
{
	SYSTICK_RVR = (CPU_FREQ / 1000) - 1;
	SYSTICK_CVR = 0;
	SYSTICK_CSR |= 0x07;
}

void systick_disable(void)
{
	SYSTICK_CSR &= ~((1 << 1) | (1 << 0));
}

uint32_t get_tick(void)
{
	return ticks;
}
