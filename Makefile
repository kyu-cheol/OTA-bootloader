CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)gcc
OBJCOPY=$(CROSS_COMPILE)objcopy

OBJS_APP=startup.o main.o interrupt_app.o system.o systick.o timer.o led.o uart.o button.o
OBJS_BL=startup_bl.o main_bl.o interrupt_bl.o system.o systick.o uart.o

LSCRIPT_APP=app.ld
LSCRIPT_BL=bootloader.ld

CFLAGS=-mcpu=cortex-m4 -mthumb -g -ggdb -Wall -Wno-main -Wstack-usage=200 -ffreestanding -Wno-main -nostdlib
LDFLAGS=-Wl,-gc-sections -mcpu=cortex-m4 -mthumb -nostartfiles

all: image.bin

image.bin: app.bin bootloader.bin
	cat bootloader.bin app.bin > image.bin

app.bin: app.elf
	$(OBJCOPY) -O binary $^ $@

bootloader.bin: bootloader.elf
	$(OBJCOPY) -O binary --pad-to=0x08001000 --gap-fill=0xFF $< $@

app.elf: $(OBJS_APP) $(LSCRIPT_APP)
	$(LD) $(LDFLAGS) $(OBJS_APP) -o $@ -Wl,-Map=app.map -T $(LSCRIPT_APP)

bootloader.elf: $(OBJS_BL) $(LSCRIPT_BL)
	$(LD) $(LDFLAGS) $(OBJS_BL) -o $@ -Wl,-Map=bootloader.map -T $(LSCRIPT_BL)
	
clean:
	rm -f *.bin *.elf *.o *.map
