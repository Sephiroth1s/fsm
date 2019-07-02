
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
#ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
#ifndef PRINT_STR_OUTPUT_BYTE
#error No defined macro PRINT_STR_OUTPUT_BYTE(__TARGET,__BYTE) for output byte, please define one with prototype bool (*)(void* pTarget,uint8_t chByte);
#endif
#else
#ifndef PRINT_STR_OUTPUT_BYTE
#error No defined macro PRINT_STR_OUTPUT_BYTE(__BYTE) for output byte, please define one with prototype bool (*)(uint8_t chByte);
#endif
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
    #ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
    this.fnPrintByte = ptCFG->fnPrintByte;
    #endif 
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
            #ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
            if (PRINT_STR_OUTPUT_BYTE(this.pTarget, *this.pchString)) {
                this.pchString++;
                this.chState = PRINT_CHECK;
            }
            #else
            if (PRINT_STR_OUTPUT_BYTE(*this.pchString)) {
                this.pchString++;
                this.chState = PRINT_CHECK;
            }
            #endif
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
