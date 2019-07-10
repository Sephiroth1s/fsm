// SPDX-License-Identifier: GPL-2.0-or-later
#include "app_cfg.h"
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*============================ INCLUDES ======================================*/
#include "./led/led.h"
#include "./uart/uart.h"
#include "./event/event.h"
#include "./critical_sector/critical_sector.h"
#include "./queue/queue.h"
#include "./check_string/check_string.h"
#include "./print_string/print_string.h"
#include "./utilities/arm/app_type.h"
#include "./check_use_peek/check_use_peek.h"
/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern void platform_init(void);

#endif
/* EOF */
