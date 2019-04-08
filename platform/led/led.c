// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include <stdbool.h>
#include "stm32f1xx_hal.h"

/*============================ MACROS ========================================*/
#define LED1_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED1_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define LED2_ON()   // todo
#define LED2_OFF()  // todo
#define ABS(__N)   ((__N) < 0 ? -(__N) : (__N))
#define _BV(__N)   ((uint32_t)1<<(__N))
#define TOP         (0x01FF)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

void led_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void set_led_gradation(uint16_t hwLevel)
{
   static uint16_t s_hwCounter = 0;
   
   if (hwLevel >= s_hwCounter) {
       LED1_ON();
   } else {
       LED1_OFF();
   }
   
   s_hwCounter++;
   s_hwCounter &= TOP;
}
 
void breath_led(void)
{
   static uint16_t s_hwCounter = 0;
   static int16_t s_nGray = (TOP >> 1);
   
   s_hwCounter++;
   if (!(s_hwCounter & (_BV(10)-1))) {
       s_nGray++;
       if (s_nGray == TOP) {
           s_nGray = 0;
       }
   }
   
   set_led_gradation(ABS(s_nGray - (TOP >> 1)));
}
