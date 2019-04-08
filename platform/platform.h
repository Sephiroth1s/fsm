// SPDX-License-Identifier: GPL-2.0-or-later
#include "app_cfg.h"
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*============================ INCLUDES ======================================*/
#include "./led/led.h"
#include "./key/key.h"
#include "./uart/uart.h"
#include "./event/event.h"
#include "./mailbox/mailbox.h"
#include "./check_string/check_string.h"
#include "./print_string/print_string.h"
#include "./critical_sector/critical_sector.h"
#include "./utilities/compiler.h"
#include "./utilities/usebits.h"
#include "./utilities/simple_fsm.h"
#include "./utilities/tiny_fsm.h"
#include "./utilities/communicate.h"
#include "./utilities/template/template.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

extern void platform_init(void);

#endif
/* EOF */
