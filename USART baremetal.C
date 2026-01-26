#include "uart_baremetal.h"

/* ============================
   STM32F411RE UART DRIVER (Bare-Metal)
   Source File
   USART2: PA2 (TX), PA3 (RX)
   ============================ */

#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)
#define APB1PERIPH_BASE   (PERIPH_BASE + 0x00000000UL)

#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define USART2_BASE       (APB1PERIPH_BASE + 0x4400UL)

#define GPIOA_MODER       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL        (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

#define RCC_AHB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x40))

#define USART2_SR         (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR         (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR        (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1        (*(volatile uint32_t *)(USART2_BASE + 0x0C))

#define RCC_AHB1ENR_GPIOAEN   (1U << 0)
#define RCC_APB1ENR_USART2EN (1U << 17)

#define USART_CR1_UE      (1U << 13)
#define USART_CR1_TE      (1U << 3)
#define USART_CR1_RE      (1U << 2)

#define USART_SR_TXE      (1U << 7)
#define USART_SR_RXNE     (1U << 5)

/* =========================================================
   USART2 INIT FUNCTION
   ========================================================= */
void UART2_Init(uint32_t baudrate)
{
    /* 1. Enable clocks for GPIOA and USART2 */
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC_APB1ENR |= RCC_APB1ENR_USART2EN;

    /* 2. Set PA2 and PA3 to Alternate Function mode */
    GPIOA_MODER &= ~(0xF << 4);   // Clear MODER2 and MODER3
    GPIOA_MODER |=  (0xA << 4);   // Set AF mode for PA2, PA3

    /* 3. Select AF7 (USART2) for PA2 and PA3 */
    GPIOA_AFRL &= ~(0xFF << 8);   // Clear AFRL2 and AFRL3
    GPIOA_AFRL |=  (0x77 << 8);   // AF7 for PA2, PA3

    /* 4. Configure baud rate (assuming 16 MHz PCLK1) */
    USART2_BRR = 16000000U / baudrate;

    /* 5. Enable transmitter and receiver */
    USART2_CR1 |= USART_CR1_TE | USART_CR1_RE;

    /* 6. Enable USART2 */
    USART2_CR1 |= USART_CR1_UE;
}

/* =========================================================
   TRANSMIT SINGLE CHARACTER (BLOCKING)
   ========================================================= */
void UART2_SendChar(char ch)
{
    while (!(USART2_SR & USART_SR_TXE));
    USART2_DR = (uint8_t)ch;
}

/* =========================================================
   TRANSMIT STRING (BLOCKING)
   ========================================================= */
void UART2_SendString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

/* =========================================================
   RECEIVE SINGLE CHARACTER (BLOCKING)
   ========================================================= */
char UART2_ReceiveChar(void)
{
    while (!(USART2_SR & USART_SR_RXNE));
    return (char)(USART2_DR & 0xFF);
}
