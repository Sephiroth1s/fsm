#include "app_cfg.h"
#ifndef __CHECK_STRING_H__
#define __CHECK_STRING_H__
#include "../queue/queue.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>  

typedef struct {
    uint8_t chState;
    uint8_t chCurrentByte;
    uint8_t *pchString;
    byte_queue_t *ptFIFOin;
} check_str_t;

extern fsm_rt_t check_string(check_str_t *ptCHK);
extern bool check_string_init(check_str_t *ptCHK, uint8_t *pchString,byte_queue_t*ptFIFOin);

#endif
