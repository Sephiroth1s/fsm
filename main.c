// SPDX-License-Identifier: GPL-2.0-or-later

/*============================ INCLUDES ======================================*/
#include "./platform/platform.h"
#include "./platform/queue/queue.h"
#include "./platform/check_use_peek/check_use_peek.h"
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

#define INPUT_FIFO_SIZE 30
#define OUTPUT_FIFO_SIZE 100
#define WORDS_NUMBER 3

#define FN_ENQUEUE_BYTE enqueue_byte
#define FN_DEQUEUE_BYTE dequeue_byte
#define FN_PEEK_BYTE_QUEUE peek_byte_queue

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
typedef struct 
{
    uint8_t chState;
    check_str_t tCheckHello;  
}check_hello_pcb_t;

typedef struct 
{
    uint8_t chState;
    check_str_t tCheckOrange;  
}check_orange_pcb_t;

typedef struct 
{
    uint8_t chState;
    check_str_t tCheckApple;
}check_apple_pcb_t;
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
static event_t s_tPrintWorld, s_tPrintApple, s_tPrintOrange;
static uint8_t s_chBytein[INPUT_FIFO_SIZE],s_chByteout[OUTPUT_FIFO_SIZE];
static byte_queue_t s_tFIFOin, s_tFIFOout;
static check_hello_pcb_t s_tCheckHelloPCB;
static check_apple_pcb_t s_tCheckApplePCB;
static check_orange_pcb_t s_tCheckOrangePCB;
/*============================ PROTOTYPES ====================================*/
/**
 * @brief  The application entry point.
 *
 * @retval None
 */

static fsm_rt_t task_print_world(void);
static fsm_rt_t task_print_apple(void);
static fsm_rt_t task_print_orange(void);

static fsm_rt_t task_world(void);
static fsm_rt_t task_apple(void);
static fsm_rt_t task_orange(void);

static fsm_rt_t check_hello(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop);
static fsm_rt_t check_apple(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop);
static fsm_rt_t check_orange(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop);

static check_agent_t c_tCheckWordsAgent[WORDS_NUMBER] = {
    {&s_tCheckHelloPCB, check_hello},
    {&s_tCheckApplePCB, check_apple},
    {&s_tCheckOrangePCB, check_orange}};
const check_use_peek_cfg_t c_tCheckWordsUsePeekCFG = {WORDS_NUMBER, &s_tFIFOin, c_tCheckWordsAgent};
static check_use_peek_t s_tCheckWordsUsePeek;

static fsm_rt_t serial_in_task(void);
static fsm_rt_t serial_out_task(void);
int main(void)
{
    platform_init();
    INIT_EVENT(&s_tPrintWorld, false, false);
    INIT_EVENT(&s_tPrintApple, false, false);
    INIT_EVENT(&s_tPrintOrange, false, false);
    INIT_BYTE_QUEUE(&s_tFIFOin, s_chBytein, sizeof(s_chBytein));
    INIT_BYTE_QUEUE(&s_tFIFOout, s_chByteout, sizeof(s_chByteout));
    check_use_peek_init(&s_tCheckWordsUsePeek,&c_tCheckWordsUsePeekCFG);
    LED1_OFF();
    while (1) {
        breath_led();
        task_print_world();
        task_print_apple();
        task_print_orange();
        task_check_use_peek(&s_tCheckWordsUsePeek);
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
    static print_str_t s_tPrintString;
    static enum {
        START,
        WAIT_PRINT,
        PRINT_WORLD
    } s_tState = START;
    switch (s_tState) {
        case START:
            do {
                const print_str_cfg_t c_tCFG = {
                    "world\r\n",
                    &s_tFIFOout,
                    FN_ENQUEUE_BYTE
                };
                print_string_init(&s_tPrintString, &c_tCFG);
            } while (0);
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrintWorld)) {
                    s_tState = PRINT_WORLD;
            }
            break;
        case PRINT_WORLD:
            if (fsm_rt_cpl == print_string(&s_tPrintString)) {
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

static fsm_rt_t task_print_apple(void)
{
    static enum {
        START,
        PRINT_APPLE
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = START;
            // break;
        case PRINT_APPLE:
            if (fsm_rt_cpl == task_apple()) {
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

static fsm_rt_t task_apple(void)
{
    static print_str_t s_tPrintString;
    static enum {
        START,
        WAIT_PRINT,
        PRINT_APPLE
    } s_tState = START;

    switch (s_tState) {
        case START:
            do {
                const print_str_cfg_t c_tCFG = {
                    "apple\r\n", 
                    &s_tFIFOout,
                    FN_ENQUEUE_BYTE
                };
                print_string_init(&s_tPrintString, &c_tCFG);
            } while (0);
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrintApple)) {
                s_tState = PRINT_APPLE;
                // break;
            } else {
                break;
            }
        case PRINT_APPLE:
            if (fsm_rt_cpl == print_string(&s_tPrintString)) {
                RESET_EVENT(&s_tPrintApple);
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

static fsm_rt_t task_print_orange(void)
{
    static enum {
        START,
        PRINT_ORANGE
    } s_tState = START;
    switch (s_tState) {
        case START:
            s_tState = START;
            // break;
        case PRINT_ORANGE:
            if (fsm_rt_cpl == task_orange()) {
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

static fsm_rt_t task_orange(void)
{
    static print_str_t s_tPrintString;
    static enum {
        START,
        WAIT_PRINT,
        PRINT_ORANGE
    } s_tState = START;
    switch (s_tState) {
        case START:
            do {
                const print_str_cfg_t c_tCFG = {
                    "orange\r\n", 
                    &s_tFIFOout,
                    FN_ENQUEUE_BYTE
                };
                print_string_init(&s_tPrintString, &c_tCFG);
            } while (0);
            s_tState = WAIT_PRINT;
            // break;
        case WAIT_PRINT:
            if (WAIT_EVENT(&s_tPrintOrange)) {
                s_tState = PRINT_ORANGE;
                // break;
            } else {
                break;
            }
        case PRINT_ORANGE:
            if (fsm_rt_cpl == print_string(&s_tPrintString)) {
                RESET_EVENT(&s_tPrintOrange);
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

fsm_rt_t check_hello(void *pTarget, read_byte_evt_handler_t *ptReadByte, bool *pbRequestDrop)
{
    check_hello_pcb_t *ptThis = (check_hello_pcb_t *)pTarget;
    enum {
        START,
        CHECK_STRING
    };
    switch (this.chState) {
        case START:
            do {
                const check_str_cfg_t c_tCFG = {
                    "hello",
                    ptReadByte
                };
                check_string_init(&this.tCheckHello, &c_tCFG);
            } while (0);
            this.chState = CHECK_STRING;
            // break;
        case CHECK_STRING:
            *pbRequestDrop = false;
            if (fsm_rt_cpl == check_string(&this.tCheckHello, pbRequestDrop)) {
                SET_EVENT(&s_tPrintWorld);
                TASK_CHECK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}

static fsm_rt_t check_apple(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop)
{
    check_apple_pcb_t *ptThis=(check_apple_pcb_t *)pTarget;
    enum {
        START,
        CHECK_STRING
    };
    switch (this.chState) {
        case START:
            do {
                const check_str_cfg_t c_tCFG = {
                    "apple", 
                    ptReadByte
                };
                check_string_init(&this.tCheckApple, &c_tCFG);
            } while (0);
            this.chState = CHECK_STRING;
            // break;
        case CHECK_STRING:
            *pbRequestDrop=false;
            if (fsm_rt_cpl == check_string(&this.tCheckApple, pbRequestDrop)) {
                SET_EVENT(&s_tPrintApple);
                TASK_CHECK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}

static fsm_rt_t check_orange(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop)
{
    check_orange_pcb_t* ptThis=(check_orange_pcb_t*)pTarget;
    enum {
        START,
        CHECK_STRING
    };
    switch (this.chState) {
        case START:
            do {
                const check_str_cfg_t c_tCFG = {
                    "orange", 
                    ptReadByte
                };
                check_string_init(&this.tCheckOrange, &c_tCFG);
            } while (0);
            this.chState = CHECK_STRING;
            // break;
        case CHECK_STRING:
            *pbRequestDrop=false;
            if (fsm_rt_cpl == check_string(&this.tCheckOrange, pbRequestDrop)) {
                SET_EVENT(&s_tPrintOrange);
                TASK_CHECK_RESET_FSM();
                return fsm_rt_cpl;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
