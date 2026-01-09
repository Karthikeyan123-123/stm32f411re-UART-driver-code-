#include "uart_driver.h"

int main(void)
{
    uart_init();

    while (1)
    {
        if (uart_rx_len)
        {
            uart_tx_dma(uart_rx_buf, uart_rx_len);
            uart_rx_len = 0;
        }
    }
}
