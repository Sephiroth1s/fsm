#include "app_cfg.h"
#ifndef __PRINT_STRING_H__
#define __PRINT_STRING_H__
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint8_t *pchString;
    uint8_t chState;
    byte_queue_t *ptFIFOout;
} print_str_t;
extern fsm_rt_t print_string(print_str_t *ptPRN);
extern bool print_string_init(print_str_t *ptPRN, uint8_t *pchString, byte_queue_t *ptFIFOout);
#endif
