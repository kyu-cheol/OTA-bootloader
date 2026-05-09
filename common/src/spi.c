#include <stdint.h>
#include "spi.h"
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"

// SPI1 pins
#define SPI1_MOSI_PIN (5)   // PB5
#define SPI1_MISO_PIN (4)   // PB4
#define SPI1_SCK_PIN  (5)   // PA5
#define SPI1_NSS_PIN  (4)   // PA4

void (*spi_rx_callback)(SPI_x *spi, uint8_t data) = 0;
void (*spi_ovr_callback)(SPI_x *spi, uint8_t data) = 0;

static void spi1_reset(void)
{
    RCC_APB2_CLOCK_RST |= SPI1_APB2_CLOCK_ER_VAL;
    __asm__ volatile ("DMB");
    RCC_APB2_CLOCK_RST &= ~SPI1_APB2_CLOCK_ER_VAL;
}

static void spi1_pins_setup(void)
{
    uint32_t reg;

    RCC_AHB1_CLOCK_ER |= GPIOA_AHB1_CLOCK_ER_VAL;
    RCC_AHB1_CLOCK_ER |= GPIOB_AHB1_CLOCK_ER_VAL;

    /* PA5, PA4 SPI AF gpio set */
    reg = GPIOA->MODE;
    reg &= ~(0x03 << (SPI1_SCK_PIN * 2));
    reg &= ~(0x03 << (SPI1_NSS_PIN * 2));
    reg |= (GPIO_MODE_AF << (SPI1_SCK_PIN * 2));
    reg |= (GPIO_MODE_AF << (SPI1_NSS_PIN * 2));
    GPIOA->MODE = reg;

    reg = GPIOA->AFRL;
    reg &= ~(0xf << (SPI1_SCK_PIN * 4));
    reg &= ~(0xf << (SPI1_NSS_PIN * 4));
    reg |= (SPI1_PIN_AF << (SPI1_SCK_PIN * 4));
    reg |= (SPI1_PIN_AF << (SPI1_NSS_PIN * 4));
    GPIOA->AFRL = reg;

    /* PB5, PB4 SPI AF gpio set */
    reg = GPIOB->MODE;
    reg &= ~(0x03 << (SPI1_MOSI_PIN * 2));
    reg &= ~(0x03 << (SPI1_MISO_PIN * 2));
    reg |= (GPIO_MODE_AF << (SPI1_MOSI_PIN * 2));
    reg |= (GPIO_MODE_AF << (SPI1_MISO_PIN * 2));
    GPIOB->MODE = reg;

    reg = GPIOB->AFRL;
    reg &= ~(0xf << (SPI1_MOSI_PIN * 4));
    reg &= ~(0xf << (SPI1_MISO_PIN * 4));
    reg |= (SPI1_PIN_AF << (SPI1_MOSI_PIN * 4));
    reg |= (SPI1_PIN_AF << (SPI1_MISO_PIN * 4));
    GPIOB->AFRL = reg;

    /* NSS pin pull-up */
    reg = GPIOA->PUPD;
    reg &= ~(0x03 << (SPI1_NSS_PIN * 2));
    reg |= (GPIO_PUPD_PU << (SPI1_NSS_PIN * 2));
    GPIOA->PUPD = reg;

    /* SPI pin 고속모드 설정 */
    GPIOA->OSPEED |= (GPIO_OSPEED_VH << (SPI1_SCK_PIN * 2)) | (GPIO_OSPEED_VH << (SPI1_NSS_PIN * 2));
    GPIOB->OSPEED |= (GPIO_OSPEED_VH << (SPI1_MOSI_PIN * 2)) | (GPIO_OSPEED_VH << (SPI1_MISO_PIN * 2));
}

void spi_init(SPI_x *spi, uint8_t mode, uint8_t size)
{
    uint32_t reg;

    if (mode == SPI_RECV_ONLY_SLAVE) {
        spi1_pins_setup();

        // SPI clock on
        RCC_APB2_CLOCK_ER |= SPI1_APB2_CLOCK_ER_VAL;

        // SPI reset
        spi1_reset();

        // SPI off
        reg = spi->CR1;
        reg &= ~SPI_CR1_SPI_EN;
        
        // set slave mode(MSTR)
        reg &= ~SPI_CR1_MASTER;

        // set CPOL/CPHA
        reg &= ~(0x03);

        // set Data frame format(8bit/16bit)
        (size == 16) ? (reg |= (1 << 11)) : (reg &= ~(1 << 11));

        // set RXONLY
        reg |= (1 << 10);

        // set Slave NSS mode(SSM/SSI)
        reg &= ~(1 << 9);
        spi->CR1 = reg;

        // RXNEIE, ERRIE(OVR) set
        reg = spi->CR2;
        reg |= SPI_CR2_RXNEI_EN | SPI_CR2_ERRI_EN;

        // reset SSOE
        reg &= ~(1 << 2);
        spi->CR2 = reg;

        // NVIC spi interrupt enable
        nvic_irq_enable(NVIC_SPI1_IRQN);
        nvic_irq_setprio(NVIC_SPI1_IRQN, 2);

        // SPI on
        spi->CR1 |= SPI_CR1_SPI_EN;
    }
}

void spi_write(SPI_x *spi, uint16_t data)
{
    ;
}

void spi_read(SPI_x *spi, uint16_t *data)
{
    ;
}

void spi_deinit(SPI_x *spi)
{
    uint32_t reg;

    // SPI interrupt disable
    reg = spi->CR2;
    reg &= ~SPI_CR2_RXNEI_EN;
    reg &= ~SPI_CR2_ERRI_EN;
    spi->CR2 = reg;

    // SPI off
    reg = spi->CR1;
    reg &= ~SPI_CR1_SPI_EN;
    spi->CR1 = reg;

    // wait busy
    reg = spi->SR;
    while (spi->SR & SPI_SR_BSY);

    // SPI reset
    spi1_reset();

    // SPI clock off
    RCC_APB2_CLOCK_ER &= ~SPI1_APB2_CLOCK_ER_VAL;

    // NVIC disable irq
    nvic_irq_disable(NVIC_SPI1_IRQN);
}

void spi_regist_rx_callback(void (*func)(SPI_x *, uint8_t))
{
    spi_rx_callback = func;
}

void spi_regist_ovr_callback(void (*func)(SPI_x *, uint8_t))
{
    spi_ovr_callback = func;
}