#include "app_cfg.h"
#ifndef __CHECK_USE_PEEK_H__
#define __CHECK_USE_PEEK_H__
#include "queue.h"
#include "../utilities/arm/app_type.h"
#include <stdbool.h>
#include <stdint.h>

typedef fsm_rt_t check_words_t(byte_queue_t *, bool *);
typedef struct
{
    void *pTarget;
    check_words_t *fnCheckWords;
} check_words_agent_t;

typedef struct {
    uint8_t chState;
    uint8_t chAgentsNumber;
    check_words_agent_t *ptAgents;
}check_use_peek_t;

typedef struct {
    uint8_t chAgentsNumber;
    check_words_agent_t *ptAgents;
} check_use_peek_cfg_t;

extern bool check_use_peek_init(check_use_peek_t *ptThis, const check_use_peek_cfg_t *ptCFG);
extern fsm_rt_t task_check_use_peek(check_use_peek_t *ptThis);
#endif
