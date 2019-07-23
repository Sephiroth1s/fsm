// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static UART_HandleTypeDef huart1;

/*============================ PROTOTYPES ====================================*/

void uart_init(void)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);
}

bool serial_out(uint8_t chByte)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) != RESET) {
        huart1.Instance->DR = chByte;
        return true;
    } else {
        return false;
    }
}

bool serial_in(uint8_t *pchByte)
{
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET) {
        *pchByte = huart1.Instance->DR;
        return true;
    } else {
        return false;
    }
}

int fputc(int ch, FILE *f) {
    while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==RESET);
    serial_out(ch);
    return ch;
}
