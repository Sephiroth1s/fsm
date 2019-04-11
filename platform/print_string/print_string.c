
#include "app_cfg.h"
#include "print_string.h"
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define this (*ptThis)
#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0)

bool print_string_init(print_str_t *ptThis,print_str_cfg_t *ptCFG)
{
    enum {
        START
    };
    if ((NULL==ptThis)||(NULL==ptCFG)){
        return false;
    }
    this.chState=ptCFG->chState;
    this.pchString=ptCFG->pchString;
    this.ptFIFOout=ptCFG->ptFIFOout;
    this.fnEnqueue=ptCFG->fnEnqueue;
    return true;
}

fsm_rt_t print_string(print_str_t *ptThis)
{
    enum {
        START,
        PRINT_CHECK,
        PRINT_STR
    };
    if (NULL==ptThis) {
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
            if ((*this.fnEnqueue)(this.ptFIFOout, *this.pchString)) {
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
