#include "app_cfg.h"
#ifndef __PRINT_STRING_H__
#define __PRINT_STRING_H__

#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include "../utilities/compiler.h"

typedef bool print_byte_t(void *, uint8_t);

typedef struct {
    uint8_t chState;
    uint8_t *pchString;
    void *pTarget;
    #ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
    print_byte_t *fnPrintByte;
    #endif
} print_str_t;

typedef struct {
    uint8_t *pchString;
    void *pTarget;
    #ifdef PRINT_STR_CFG_USE_FUNCTION_POINTER
    print_byte_t *fnPrintByte;
    #endif
} print_str_cfg_t;

#define PRINT_STR_POOL_ITEM_SIZE sizeof(print_str_t)
#define PRINT_STR_POOL_ITEM_COUNT 8

typedef struct print_str_pool_item_t print_str_pool_item_t;
struct print_str_pool_item_t {
    uint8_t chBuffer[PRINT_STR_POOL_ITEM_SIZE];
    bool bIsFree;
} ALIGN(__ALIGNOF__(print_str_t));

print_str_pool_item_t *print_str_pool_allocate(void);
void print_str_pool_free(print_str_pool_item_t *ptItem);
extern void print_str_pool_item_init(void);

extern fsm_rt_t print_string(print_str_t *ptThis);
extern bool print_string_init(print_str_t *ptThis, const print_str_cfg_t *ptCFG);
#endif
