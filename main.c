// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include "./platform/platform.h"
#include "./platform/queue/queue.h"
#include "./utilities/compiler.h"

/*============================ MACROS ========================================*/
#define this (*ptThis)
#define TASK_CHECK_RESET_FSM()      \
    do {                      \
        this.chState = START; \
    } while (0)
#define TASK_RESET_FSM()  \
    do {                  \
        s_tState = START; \
    } while (0)

#define OUTPUT_FIFO_SIZE 100

#define FN_ENQUEUE_BYTE (&enqueue_byte)
#define FN_DEQUEUE_BYTE (&dequeue_byte)
#define FN_PEEK_BYTE_QUEUE (&peek_byte_queue)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static event_t s_tPrintWorld;
static uint8_t s_chByteout[OUTPUT_FIFO_SIZE];
static byte_queue_t s_tFIFOin, s_tFIFOout;
/*============================ PROTOTYPES ====================================*/
/**
 * @brief  The application entry point.
 *
 * @retval None
 */

static fsm_rt_t task_print_world(void);
static fsm_rt_t serial_out_task(void);
static fsm_rt_t task_world(void);


int main(void)
{
    platform_init();
    print_str_pool_item_init();
    INIT_EVENT(&s_tPrintWorld, false, false);
    INIT_BYTE_QUEUE(&s_tFIFOout, s_chByteout, sizeof(s_chByteout));
    LED1_OFF();
    SET_EVENT(&s_tPrintWorld);
    while (1) {
        breath_led();
        task_world();
        //task_print_world();
        serial_out_task();
    }
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

static fsm_rt_t task_print_world(void)
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
            if (fsm_rt_cpl == task_world()) {
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

static fsm_rt_t task_world(void)
{
    static print_str_pool_item_t *s_ptPrintString;
    static enum {
        START,
        INIT,
        WAIT_PRINT,
        PRINT_WORLD
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = INIT;
            //break;
        case INIT:
            s_ptPrintString=print_str_pool_allocate();
            if (s_ptPrintString==NULL)
            {
                TASK_RESET_FSM();
                break;
            }
            do {
                const print_str_cfg_t c_tCFG = {
                    "world\r\n",
                    &s_tFIFOout,
                    (&enqueue_byte)
                };
                print_string_init((print_str_t*)s_ptPrintString->chBuffer, &c_tCFG);
            } while (0);
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrintWorld)) {
                s_tState = PRINT_WORLD;
            }
            break;
        case PRINT_WORLD:
            if (fsm_rt_cpl == print_string((print_str_t*)s_ptPrintString->chBuffer)) {
                print_str_pool_free(s_ptPrintString);
                RESET_EVENT(&s_tPrintWorld);
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

