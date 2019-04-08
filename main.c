// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include "./platform/platform.h"
/*============================ MACROS ========================================*/
#define this (*ptThis)
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static event_t s_tPrint;
static byte_queue_t s_tFIFOin;
/*============================ PROTOTYPES ====================================*/

/**
 * @brief  The application entry point.
 *
 * @retval None
 */


static fsm_rt_t print_hello(void);
static fsm_rt_t check_world(void);
static fsm_rt_t task_check(void);
static fsm_rt_t task_print(void);

int main(void) 
{
    platform_init();
    INIT_EVENT(&s_tPrint, false, false);
    LED1_OFF();
    while (1) {
        breath_led();
        task_check();
        task_print();
    }
}

static fsm_rt_t task_check(void) 
{
    static enum { 
        START, 
        CHECK_WORLD 
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = CHECK_WORLD;
            // break;
        case CHECK_WORLD:
            if (fsm_rt_cpl == check_world()) {
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
static fsm_rt_t task_print(void) 
{
    static enum { 
        START, 
        PRINT_HELLO 
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = START;
            // break;
        case PRINT_HELLO:
            if (fsm_rt_cpl == print_hello()) {
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}

static fsm_rt_t check_world(void) 
{
    static check_str_t s_tCheckWorld;
    static enum { 
        START, 
        CHECK_STRING 
    } s_tState = START;
    switch (s_tState) {
        case START:
            check_string_init(&s_tCheckWorld, "world");
            s_tState = CHECK_STRING;
            // break;
        case CHECK_STRING:
            if (fsm_rt_cpl == check_string(&s_tCheckWorld)) {
                SET_EVENT(&s_tPrint);
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}

static fsm_rt_t print_hello(void) 
{
    static print_str_t s_tPrintString;
    static enum { 
        START, 
        WAIT_PRINT, 
        PRINT_HELLO 
    } s_tState = START;
    switch (s_tState) {
        case START:
            print_string_init(&s_tPrintString, "hello");
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrint)) {
                s_tState = PRINT_HELLO;
                // break;
            } else {
                break;
            }
        case PRINT_HELLO:
            if (fsm_rt_cpl == print_string(&s_tPrintString)) {
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}

