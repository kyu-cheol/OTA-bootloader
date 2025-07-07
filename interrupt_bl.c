#include <stdint.h>
#include "timer.h"

volatile uint32_t ticks;

void isr_systick(void)
{
	ticks++;
}

