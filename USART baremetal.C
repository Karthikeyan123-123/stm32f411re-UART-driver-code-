#include "uart_driver.h"

/* ================= RCC ================= */
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)

/* ================= GPIO ================= */
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_AFRL    (*(volatile uint32_t*)0x40020020)

/* ================= USART2 ================= */
#define USART2_SR     (*(volatile uint32_t*)0x40004400)
#define USART2_DR     (*(volatile uint32_t*)0x40004404)
#define USART2_BRR    (*(volatile uint32_t*)0x40004408)
#define USART2_CR1    (*(volatile uint32_t*)0x4000440C)
#define USART2_CR3    (*(volatile uint32_t*)0x40004414)

/* ================= DMA1 ================= */
#define DMA1_S5CR     (*(volatile uint32_t*)0x40026088)  // RX
#define DMA1_S5NDTR   (*(volatile uint32_t*)0x4002608C)
#define DMA1_S5PAR    (*(volatile uint32_t*)0x40026090)
#define DMA1_S5M0AR   (*(volatile uint32_t*)0x40026094)

#define DMA1_S6CR     (*(volatile uint32_t*)0x400260A0)  // TX
#define DMA1_S6NDTR   (*(volatile uint32_t*)0x400260A4)
#define DMA1_S6PAR    (*(volatile uint32_t*)0x400260A8)
#define DMA1_S6M0AR   (*(volatile uint32_t*)0x400260AC)

#define DMA1_HIFCR    (*(volatile uint32_t*)0x4002601C)

/* ================= DATA ================= */
uint8_t uart_rx_buf[RX_BUF_SIZE];
volatile uint16_t uart_rx_len = 0;

/* ================= INIT ================= */
void uart_init(void)
{
    /* Clocks */
    RCC_AHB1ENR |= (1 << 0) | (1 << 21);   // GPIOA, DMA1
    RCC_APB1ENR |= (1 << 17);              // USART2

    /* PA2 TX, PA3 RX */
    GPIOA_MODER &= ~(0xF << 4);
    GPIOA_MODER |=  (0xA << 4);
    GPIOA_AFRL  |=  (0x77 << 8);

    /* USART */
    USART2_BRR = 0x683;  // 9600 @ 16 MHz
    USART2_CR1 |= (1 << 13) | (1 << 2) | (1 << 3); // UE, RE, TE
    USART2_CR1 |= (1 << 4); // IDLE interrupt
    USART2_CR3 |= (1 << 6); // DMAR

    /* RX DMA */
    DMA1_S5CR &= ~1;
    DMA1_S5PAR  = (uint32_t)&USART2_DR;
    DMA1_S5M0AR = (uint32_t)uart_rx_buf;
    DMA1_S5NDTR = RX_BUF_SIZE;
    DMA1_S5CR =
        (4 << 25) |   // Channel 4
        (1 << 10) |   // MINC
        (2 << 16);    // High priority
    DMA1_S5CR |= 1;
}

/* ================= TX DMA ================= */
void uart_tx_dma(uint8_t *buf, uint16_t len)
{
    DMA1_S6CR &= ~1;
    while (DMA1_S6CR & 1);

    DMA1_HIFCR = (0x3F << 16);

    DMA1_S6PAR  = (uint32_t)&USART2_DR;
    DMA1_S6M0AR = (uint32_t)buf;
    DMA1_S6NDTR = len;

    DMA1_S6CR =
        (4 << 25) |   // Channel 4
        (1 << 10) |   // MINC
        (1 << 6)  |   // M2P
        (2 << 16);

    USART2_CR3 |= (1 << 7); // DMAT
    DMA1_S6CR |= 1;
}
