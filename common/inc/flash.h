#ifndef __FLASH_H__
#define __FLASH_H__

#include <stdint.h>

typedef struct FLASH_INTERFACE {
    uint32_t ACR;
    uint32_t KEYR;
    uint32_t OPTKEYR;
    uint32_t SR;
    uint32_t CR;
    uint32_t OPTCR;
} FLASH_interface;

#define FLASH ((FLASH_interface *)0x40023C00)

// FLASH register setting
#define FLASH_ACR_ENABLE_DATA_CACHE (1 << 10)
#define FLASH_ACR_ENABLE_INST_CACHE (1 << 9)
#define FLASH_ACR_ENABLE_PREFETCH   (1 << 8)

/* FLASH interface prototypes */
void flash_init(void);
void flash_lock(void);
void flash_unlock(void);
void flash_erase(uint8_t sector_num);
void flash_write_word(uint32_t start_addr, uint32_t data);

#endif