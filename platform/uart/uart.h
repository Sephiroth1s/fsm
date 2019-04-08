// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef __UART_H__
#define __UART_H__

/*============================ INCLUDES ======================================*/
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern void uart_init(void);
extern bool serial_out(uint8_t chByte);
extern bool serial_in(uint8_t *pchByte);

#endif
/* EOF */

