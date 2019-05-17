#include "app_cfg.h"
#ifndef __PRINT_STRING_H__
#define __PRINT_STRING_H__
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>

typedef bool print_byte_t(void *, uint8_t);

typedef struct {
    uint8_t chState;
    uint8_t *pchString;
    void *pTarget;
    print_byte_t *fnPrintByte;
} print_str_t;

typedef struct {
    uint8_t *pchString;
    void *pTarget;
    print_byte_t *fnPrintByte;
} print_str_cfg_t;

extern fsm_rt_t print_string(print_str_t *ptThis);
extern bool print_string_init(print_str_t *ptThis, const print_str_cfg_t *ptCFG);
#endif
