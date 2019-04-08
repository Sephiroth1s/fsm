#include "app_cfg.h"
#ifndef __PRINT_STRING_H__
#define __PRINT_STRING_H__
#define this (*ptThis)
#define TASK_STR_RESET_FSM()  \
    do {                      \
        this.chState = START; \
    } while (0)
    
typedef struct {
    uint8_t *pchString;
    uint8_t chState;
} print_str_t;
extern fsm_rt_t print_string(print_str_t *ptPRN);
extern bool print_string_init(print_str_t *ptPRN, uint8_t *pchString);
#endif
