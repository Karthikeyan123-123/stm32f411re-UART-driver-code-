#ifndef __UART_BAREMETAL_H
#define __UART_BAREMETAL_H

#include <stdint.h>

/* ============================
   STM32F411RE UART DRIVER (Bare-Metal)
   Header File
   USART2: PA2 (TX), PA3 (RX)
   ============================ */

void UART2_Init(uint32_t baudrate);

// Blocking mode APIs
void UART2_SendChar(char ch);
void UART2_SendString(const char *str);
char UART2_ReceiveChar(void);

#endif /* __UART_BAREMETAL_H */
