
#include "app_cfg.h"
#include "print_string.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include "../queue/queue.h"
#include "uart.h"
#define this (*ptThis)

#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0);
#ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
#ifndef PRINT_STR_OUTPUT_BYTE
#error No defined macro PRINT_STR_OUTPUT_BYTE(__TARGET,__BYTE) for output byte, please define one with prototype bool (*)(void* pTarget,uint8_t chByte);
#endif
#else
#ifndef PRINT_STR_OUTPUT_BYTE
#error No defined macro PRINT_STR_OUTPUT_BYTE(__BYTE) for output byte, please define one with prototype bool (*)(uint8_t chByte);
#endif
#endif

static print_str_pool_item_t s_tPrintStringPool[PRINT_STR_POOL_ITEM_COUNT];
static uint8_t s_chAllocateLength = PRINT_STR_POOL_ITEM_COUNT;

bool print_string_init(print_str_t *ptThis, const print_str_cfg_t *ptCFG)
{
    enum {
        START
    };
    if ((NULL == ptThis) || (NULL == ptCFG) || (NULL == ptCFG->fnPrintByte)) {
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
    // if (NULL == ptThis || (this.fnPrintByte == NULL) || (NULL == this.pTarget)) {
    if (NULL == ptThis) {
        return fsm_rt_err;
    }
    // if (NULL == this.fnPrintByte) {
    //     while(!serial_out('-'));
    //     return fsm_rt_err;
    // }
    // if (NULL == this.pTarget) {
    //     while(!serial_out('='));
    //     return fsm_rt_err;
    // }
    //未知bug:有上两行检测则工作正常，下方不能添加串口输出否则依旧无法正常工作
    //while(!serial_out('A'));
    switch (this.chState) {
        case START:
            // while(!serial_out('1'));
            this.chState = PRINT_CHECK;
            // break;
        case PRINT_CHECK:
            // while(!serial_out('2'));
            if ('\0' == *this.pchString) {
                while(!serial_out('3'));
                TASK_STR_RESET_FSM();
                return fsm_rt_cpl;
            } else {
                // while(!serial_out('4'));
                this.chState = PRINT_STR;
            }
            // break;
        case PRINT_STR:
            // while(!serial_out('5'));
            #ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
            if (PRINT_STR_OUTPUT_BYTE(this.pTarget, *this.pchString)) {
                while(!serial_out('6'));
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

void print_str_pool_item_init(void)
{
    uint8_t chAllocateCounter = 0;
    while (chAllocateCounter < PRINT_STR_POOL_ITEM_COUNT) {
        s_tPrintStringPool[chAllocateCounter].bIsFree = true;
        chAllocateCounter++;
    }
}

print_str_pool_item_t *print_str_pool_allocate(void)
{
    static uint8_t s_chAllocateIndex = 0;
    if (!s_chAllocateLength) {
        return NULL;
    }
    while (s_chAllocateIndex < PRINT_STR_POOL_ITEM_COUNT) {
        if (s_tPrintStringPool[s_chAllocateIndex].bIsFree) {
            s_tPrintStringPool[s_chAllocateIndex].bIsFree = false;
            s_chAllocateLength--;
            return &s_tPrintStringPool[s_chAllocateIndex];
        }
        s_chAllocateIndex++;
        if (s_chAllocateIndex >= PRINT_STR_POOL_ITEM_COUNT) {
            s_chAllocateIndex = 0;
        }
    }
    return NULL;
}

void print_str_pool_free(print_str_pool_item_t *ptItem)
{
    if (ptItem != NULL) {
        ptItem->bIsFree = true;
        memset(ptItem->chBuffer, 0, PRINT_STR_POOL_ITEM_SIZE);
        s_chAllocateLength++;
    }
}
