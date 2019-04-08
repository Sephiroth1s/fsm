// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef __LED_H__
#define __LED_H__

/*============================ INCLUDES ======================================*/
#include "stm32f1xx_hal.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define LED1_ON()   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED1_OFF()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define LED2_ON()   //todo
#define LED2_OFF()  //todo

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

/*! \brief led init
 *! \param none
 *! \return none
 */
extern void led_init(void);

/*! \brief led breath
 *! \param none
 *! \return none
 */
extern void breath_led(void);

#endif
/* EOF */
