# STM32F411RE UART Driver (Bare‑Metal)

This repository contains a clean and reusable **bare‑metal UART (USART) driver** for the STM32F411RE microcontroller.
The driver is written using **direct register‑level programming** (no HAL, no CubeMX dependencies) and is ideal for learning low‑level firmware development and debugging.

---

## ✨ Features

* Pure register‑level (bare‑metal) implementation
* USART2 initialization (PA2 = TX, PA3 = RX)
* Configurable baud rate (default: 9600)
* Blocking transmit and receive
* Transmit single character
* Transmit string
* Receive single character (blocking)
* Simple and readable API
* No HAL or CubeMX dependency

---

## 📁 Files

* `uart_baremetal.h`  → Public APIs and macros
* `uart_baremetal.c`  → UART bare‑metal implementation
* `README.md`         → Project documentation

---

## 🔧 Default Configuration

* MCU: STM32F411RE (ARM Cortex‑M4)
* UART instance: USART2
* TX pin: PA2 (Alternate Function AF7)
* RX pin: PA3 (Alternate Function AF7)
* Baud rate: 9600
* Word length: 8‑bit
* Stop bits: 1
* Parity: None
* Mode: Transmit + Receive
* Flow control: None

---

## 📌 API Overview

```c
void UART2_Init(uint32_t baudrate);
void UART2_SendChar(char ch);
void UART2_SendString(const char *str);
char UART2_ReceiveChar(void);
```

---

## 🧪 Example Usage (main.c)

```c
#include "uart_baremetal.h"

int main(void)
{
    UART2_Init(9600);

    UART2_SendString("Hello from STM32F411RE (Bare‑Metal UART)!\r\n");

    while (1)
    {
        char ch = UART2_ReceiveChar();
        UART2_SendChar(ch);   // Echo received character
    }
}
```

---

## 🧠 How It Works (High Level)

1. Enable clocks for GPIOA and USART2 using RCC registers
2. Configure PA2 and PA3 as Alternate Function 7 (USART2)
3. Set the baud rate using the BRR register
4. Enable the transmitter and receiver (TE and RE bits)
5. Enable the USART peripheral (UE bit)

---

## ⚠️ Notes

* This driver uses **blocking I/O** only.
* `UART2_ReceiveChar()` waits until a byte is received.
* Suitable for debugging, CLI, bootloaders, and learning purposes.
* For multitasking or RTOS systems, consider interrupt or DMA extensions.

---

## 🚀 Future Extensions

* Interrupt‑based UART driver
* DMA‑based UART driver
* Ring buffer for RX/TX
* printf redirection support
* Support for USART1 / USART6

---

## 👤 Author

Karthikeyan

---

## 📜 License

MIT License
