
#include "app_cfg.h"
#include "check_string.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"

#define this (*ptThis)
#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0)

bool check_string_init(check_str_t *ptCHK, uint8_t *pchString, byte_queue_t *ptFIFOin, dequeue_byte_t *fndequeue)
{
    enum {
        START
    };
    if ((NULL == ptCHK) || (NULL == pchString) || (NULL == ptFIFOin) || (NULL == fndequeue)) {
        return false;
    }
    ptCHK->pchString = pchString;
    ptCHK->chState = START;
    ptCHK->ptFIFOin = ptFIFOin;
    ptCHK->fnDequeue = fndequeue;
    return true;
}

fsm_rt_t check_string(check_str_t *ptCHK)
{
    enum {
        START,
        CHECK_END,
        READ_CHAR,
        CHECK_WORLD
    };
    check_str_t *ptThis = ptCHK;
    if (ptThis == NULL) {
        return fsm_rt_err;
    }
    switch (this.chState) {
        case START:
            this.chState = CHECK_END;
            // break;
        case CHECK_END:
            if (*this.pchString == '\0') {
                TASK_STR_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                this.chState = READ_CHAR;
            }
            // break;
        case READ_CHAR:
            if ((*this.fnDequeue)(this.ptFIFOin, &this.chCurrentByte)) {
                this.chState = CHECK_WORLD;
                // break;
            } else {
                break;
            }
        case CHECK_WORLD:
            if (*this.pchString == this.chCurrentByte) {
                this.pchString++;
                this.chState = CHECK_END;
            } else {
                TASK_STR_RESET_FSM();
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
