#ifndef UART_DRIVER_HAL_H
#define UART_DRIVER_HAL_H

#include "stm32f4xx_hal.h"

#define RX_BUF_SIZE 64

void uart_hal_init(void);

extern UART_HandleTypeDef huart2;
extern uint8_t uart_rx_buf[RX_BUF_SIZE];
extern volatile uint16_t uart_rx_len;

#endif
