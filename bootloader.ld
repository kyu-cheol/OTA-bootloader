MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 0x00001000
	RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} > FLASH

	.text :
	{
		_start_text = .;
		KEEP(*(.text*))
		*(.rodata*)
		. = ALIGN(4);
		_end_text = .;
	} > FLASH

	_stored_data = .;

	.data : AT (_stored_data)
	{
		_start_data = .;
		KEEP(*(.data*))
		. = ALIGN(4);
		_end_data = .;
	} > RAM

	.bss (NOLOAD) :
	{
		_start_bss = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_end_bss = .;
		_end = .;
	} > RAM
	. = ALIGN(4);
}

END_STACK = ORIGIN(RAM) + LENGTH(RAM);
