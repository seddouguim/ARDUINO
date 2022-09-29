#include "config.h"

float min_temperature = 57.5;
float max_temperature = 62.5;

float PON = 62.7;
float POFF = 57.7;

void SERCOM0_Handler()
{
    Screen.IrqHandler();
}

Uart Screen(&sercom0, 5, 6, SERCOM_RX_PAD_1, UART_TX_PAD_0);