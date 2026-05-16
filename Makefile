# 빌드 도구 설정
CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

# 디렉토리 설정
BUILD_DIR = build
COMMON_DIR = common
APP_DIR = app
BL_DIR = bootloader

# 컴파일 및 링크 플래그 (통합 사용)
CFLAGS = -mcpu=cortex-m4 -mthumb -g -ggdb -Wall -Wno-main -Wstack-usage=200 -ffreestanding #-nostdlib
CFLAGS += -I$(COMMON_DIR)/inc -I$(APP_DIR)/inc -I$(BL_DIR)/inc
LDFLAGS = -Wl,-gc-sections -mcpu=cortex-m4 -mthumb -nostartfiles
LDFLAGS += --specs=nano.specs -lc -lm -lnosys

# 파일 목록 자동 생성
SRCS_COMMON = $(wildcard $(COMMON_DIR)/src/*.c)
SRCS_APP = $(wildcard $(APP_DIR)/src/*.c)
SRCS_BL = $(wildcard $(BL_DIR)/src/*.c)

# 빌드 타겟
# WSL -> Windows file system copy
all: $(BUILD_DIR)/image.bin
	cp $^ /mnt/c/wsl2\ workspace/image.bin

$(BUILD_DIR)/image.bin: $(BUILD_DIR)/app.bin $(BUILD_DIR)/bootloader.bin
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/app.bin > $@

$(BUILD_DIR)/app.bin: $(BUILD_DIR)/app.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/bootloader.bin: $(BUILD_DIR)/bootloader.elf
	$(OBJCOPY) -O binary --pad-to=0x08010000 --gap-fill=0xFF $< $@

# .o 파일을 만들지 않고 .c 파일들로부터 직접 .elf 생성
$(BUILD_DIR)/app.elf: $(SRCS_COMMON) $(SRCS_APP) $(APP_DIR)/app.ld | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS_COMMON) $(SRCS_APP) -o $@ $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/app.map -T $(APP_DIR)/app.ld

$(BUILD_DIR)/bootloader.elf: $(SRCS_COMMON) $(SRCS_BL) $(BL_DIR)/bootloader.ld | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS_COMMON) $(SRCS_BL) -o $@ $(LDFLAGS) -Wl,-Map=$(BUILD_DIR)/bootloader.map -T $(BL_DIR)/bootloader.ld

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf /mnt/c/wsl2\ workspace/image.bin 
