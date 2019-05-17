// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include "./platform/platform.h"
/*============================ MACROS ========================================*/
#define this (*ptThis)
#define TASK_RESET_FSM()  \
    do {                  \
        s_tState = START; \
    } while (0)
#define SIZE 100
#define FN_ENQUEUE_BYTE enqueue_byte
#define FN_DEQUEUE_BYTE dequeue_byte
#define FN_PEEK_BYTE_QUEUE peek_byte_queue
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static event_t s_tPrint;
static uint8_t s_chBytein[SIZE];
static uint8_t s_chByteout[SIZE];
static byte_queue_t s_tFIFOin, s_tFIFOout;
/*============================ PROTOTYPES ====================================*/

/**
 * @brief  The application entry point.
 *
 * @retval None
 */

static fsm_rt_t task_print_world(void);
static fsm_rt_t check_world(void);
static fsm_rt_t task_check(void);
static fsm_rt_t task_print(void);
static fsm_rt_t serial_in_task(void);
static fsm_rt_t serial_out_task(void);
static fsm_rt_t task_check_use_peek(void);
int main(void)
{
    platform_init();
    INIT_EVENT(&s_tPrint, false, false);
    INIT_BYTE_QUEUE(&s_tFIFOin, s_chBytein, SIZE);
    INIT_BYTE_QUEUE(&s_tFIFOout, s_chByteout, SIZE);
    LED1_OFF();
    while (1) {
        breath_led();
        task_check();
        task_print();
        serial_in_task();
        serial_out_task();
    }
}
fsm_rt_t serial_in_task(void)
{
    static enum {
        START,
        REDA_AND_ENQUEUE
    } s_tState = START;
    uint8_t chByte;
    switch (s_tState) {
        case START:
            s_tState = START;
            //break;
        case REDA_AND_ENQUEUE:
            if (serial_in(&chByte)) {
                ENQUEUE_BYTE(&s_tFIFOin, chByte);
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
fsm_rt_t serial_out_task(void)
{
    static enum {
        START,
        DEQUEUE,
        SEND_BYTE
    } s_tState = START;
    static uint8_t s_chByte;
    switch (s_tState) {
        case START:
            s_tState = START;
            //break;
        case DEQUEUE:
            if (!DEQUEUE_BYTE(&s_tFIFOout, &s_chByte)) {
                break;
            } else {
                s_tState = SEND_BYTE;
            }
            //break;
        case SEND_BYTE:
            if (serial_out(s_chByte)) {
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
static fsm_rt_t task_check(void)
{
    static enum {
        START,
        CHECK_HELLO
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = CHECK_HELLO;
            // break;
        case CHECK_HELLO:
            if (fsm_rt_cpl == task_check_use_peek()) {
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
        PRINT_WORLD
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = START;
            // break;
        case PRINT_WORLD:
            if (fsm_rt_cpl == task_print_world()) {
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

static fsm_rt_t task_print_world(void)
{
    static print_str_t s_tPrintString;
    static enum {
        START,
        WAIT_PRINT,
        PRINT_WORLD
    } s_tState = START;

    switch (s_tState) {
        case START:
            do {
                const print_str_cfg_t tPrintStringCFG = {
                    "world\r\n", 
                    &s_tFIFOout, 
                    FN_ENQUEUE_BYTE
                };
                print_string_init(&s_tPrintString, &tPrintStringCFG);
            } while (0);
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrint)) {
                s_tState = PRINT_WORLD;
                // break;
            } else {
                break;
            }
        case PRINT_WORLD:
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

fsm_rt_t task_check_use_peek(void)
{
    static check_str_t s_tCheckHello;
    static enum {
        START,
        CHECK_STRING
    } s_tState = START;
    static bool bIsRequestDrop;
    uint8_t chByteDrop;
    switch (s_tState) {
        case START:
            do {
                const check_str_cfg_t tCheckHelloCFG = {
                    "hello", 
                    &s_tFIFOin, 
                    FN_PEEK_BYTE_QUEUE
                };
                check_string_init(&s_tCheckHello, &tCheckHelloCFG);
            } while (0);
            s_tState = CHECK_STRING;
            // break;
        case CHECK_STRING:
            if (fsm_rt_cpl == check_string(&s_tCheckHello,&bIsRequestDrop)) {
                GET_ALL_PEEKED_BYTE(s_tCheckHello.pTarget);
                SET_EVENT(&s_tPrint);
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            if(bIsRequestDrop){
                DEQUEUE_BYTE(s_tCheckHello.pTarget,&chByteDrop);
            } 
            break;
        default:
            return fsm_rt_err;
            break;
    }
    
    return fsm_rt_on_going;
}

