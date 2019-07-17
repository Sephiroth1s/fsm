
#include "app_cfg.h"
#include "print_string.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>

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

bool print_string_init(void *pTarget, const print_str_cfg_t *ptCFG)
{
    enum {
        START=0
    };
    print_str_t *ptThis=(print_str_t *)pTarget;
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

fsm_rt_t print_string(void *pTarget)
{
    enum {
        START = 0,
        PRINT_CHECK,
        PRINT_STR
    };
    print_str_t *ptThis=(print_str_t *)pTarget;
    if (NULL == ptThis) {
        return fsm_rt_err;
    }
    switch (this.chState) {
        case START:
            this.chState = PRINT_CHECK;
            // break;
        case PRINT_CHECK:
            if ('\0' == *this.pchString) {
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

void print_str_pool_item_init(void)
{
    uint8_t chAllocateCounter = 0;
    while (chAllocateCounter < PRINT_STR_POOL_ITEM_COUNT) {
        s_tPrintStringPool[chAllocateCounter].bIsFree = true;
        memset(s_tPrintStringPool[chAllocateCounter].chBuffer, 0, PRINT_STR_POOL_ITEM_SIZE);
        chAllocateCounter++;
        printf("初始化内存池成功\r\n");
    }
}

print_str_pool_item_t *print_str_pool_allocate(void)
{
    uint8_t chAllocateCounter = 0;
    while (chAllocateCounter < PRINT_STR_POOL_ITEM_COUNT)
    {
        if (s_tPrintStringPool[chAllocateCounter].bIsFree) {
            s_tPrintStringPool[chAllocateCounter].bIsFree = false;
            printf("分配内存成功\r\n");
            return &s_tPrintStringPool[chAllocateCounter];
        }
        chAllocateCounter++;
    }
    return NULL;
}

void print_str_pool_free(print_str_pool_item_t *ptItem)
{
    if (ptItem != NULL) {
        ptItem->bIsFree = true;
        memset(ptItem->chBuffer,0,PRINT_STR_POOL_ITEM_SIZE);
        printf("释放内存\r\n");
    }
}
