#include "app_cfg.h"
#ifndef __PRINT_STRING_H__
#define __PRINT_STRING_H__
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef bool enqueue_byte_t(byte_queue_t *, uint8_t);

typedef struct {
    uint8_t chState;
    uint8_t *pchString;
    byte_queue_t *ptFIFOout;
    enqueue_byte_t *fnEnqueue;
} print_str_t;

typedef struct {
    uint8_t chState;
    uint8_t *pchString;
    byte_queue_t *ptFIFOout;
    enqueue_byte_t *fnEnqueue;
} print_str_cfg_t;

extern fsm_rt_t print_string(print_str_t *ptThis);
extern bool print_string_init(print_str_t *ptThis,print_str_cfg_t *ptCFG);
#endif
