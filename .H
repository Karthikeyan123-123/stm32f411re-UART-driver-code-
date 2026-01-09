#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

#define RX_BUF_SIZE 64

void uart_init(void);
void uart_tx_dma(uint8_t *buf, uint16_t len);

extern volatile uint16_t uart_rx_len;
extern uint8_t uart_rx_buf[RX_BUF_SIZE];

#endif
