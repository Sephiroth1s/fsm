
#include "app_cfg.h"
#include "print_string.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>

#define this (*ptThis)

#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0)

#ifndef FN_ENQUEUE_BYTE
#error No defined macro FN_ENQUEUE_BYTE for print_string
#endif

#ifndef FN_DEQUEUE_BYTE
#error No defined macro FN_DEQUEUE_BYTE for print_string
#endif

#ifndef FN_PEEK_BYTE_QUEUE
#warning No defined macro FN_PEEK_BYTE_QUEUE for print_string, default value peek_byte_queue?is?used.
#define FN_PEEK_BYTE_QUEUE peek_byte_queue
#endif


bool print_string_init(print_str_t *ptThis, const print_str_cfg_t *ptCFG)
{
    enum {
        START
    };
    if ((NULL == ptThis) || (NULL == ptCFG)) {
        return false;
    }
    this.chState = START;
    this.pchString = ptCFG->pchString;
    this.pTarget = ptCFG->pTarget;
    this.fnPrintByte = ptCFG->fnPrintByte;
    return true;
}

fsm_rt_t print_string(print_str_t *ptThis)
{
    enum {
        START,
        PRINT_CHECK,
        PRINT_STR
    };
    if (NULL == ptThis) {
        return fsm_rt_err;
    }
    switch (this.chState) {
        case START:
            this.chState = PRINT_CHECK;
            // break;
        case PRINT_CHECK:
            if (*this.pchString == '\0') {
                TASK_STR_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                this.chState = PRINT_STR;
            }
            // break;
        case PRINT_STR:
            if ((*this.fnPrintByte)(this.pTarget, *this.pchString)) {
                this.pchString++;
                this.chState = PRINT_CHECK;
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
