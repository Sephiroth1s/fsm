#include "app_cfg.h"
#ifndef __CHECK_STRING_H__
#define __CHECK_STRING_H__
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef bool read_byte_t(void *, uint8_t *);

typedef struct {
    uint8_t chState;
    uint8_t chCurrentByte;
    uint8_t *pchString;
    void *ptUserDate;
    read_byte_t *fnReadByte;
} check_str_t;

typedef struct {
    uint8_t *pchString;
    void *ptUserDate;
    read_byte_t *fnReadByte;
} check_str_cfg_t;

extern fsm_rt_t check_string(check_str_t *ptThis);
extern bool check_string_init(check_str_t *ptThis, const check_str_cfg_t *ptCFG);

#endif
