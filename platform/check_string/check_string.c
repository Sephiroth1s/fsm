
#include "app_cfg.h"
#include "check_string.h"
#include "../utilities/arm/app_type.h"
#include <stdbool.h>
#include <stdint.h>
#define this (*ptThis)
#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0)

bool check_string_init(check_str_t *ptThis, const check_str_cfg_t *ptCFG)
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
    this.fnReadByte = ptCFG->fnReadByte;
    return true;
}

fsm_rt_t check_string(check_str_t *ptThis,bool *pbIsRequestDrop)
{
    enum {
        START,
        CHECK_END,
        READ_CHAR,
        CHECK_WORLD
    };

    if (NULL == ptThis) {
        return fsm_rt_err;
    }
    switch (this.chState) {
        case START:
            this.chState = CHECK_END;
            // break;
        case CHECK_END:
            GOTO_CHECK_END:
            if (*this.pchString == '\0') {
                TASK_STR_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                this.chState = READ_CHAR;
            }
            // break;
        case READ_CHAR:
            if ((*this.fnReadByte)(this.pTarget, &this.chCurrentByte)) {
                this.chState = CHECK_WORLD;
                // break;
            } else {
                break;
            }
        case CHECK_WORLD:
            if (*this.pchString == this.chCurrentByte) {
                this.pchString++;
                this.chState = CHECK_END;
                goto GOTO_CHECK_END;
            } else {
                *pbIsRequestDrop=true;
                TASK_STR_RESET_FSM();
            }
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
