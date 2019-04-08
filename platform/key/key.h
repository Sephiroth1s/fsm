// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef __KEY_H__
#define __KEY_H__

/*============================ INCLUDES ======================================*/
#include "stm32f1xx_hal.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
#define IS_KEY1_DOWN()  (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
#define IS_KEY1_UP()    (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
#define IS_KEY2_DOWN()  //todo
#define IS_KEY2_UP()    //todo

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

/*! \brief key init
 *! \param none
 *! \return none
 */
extern void key_init(void);

#endif
/* EOF */

