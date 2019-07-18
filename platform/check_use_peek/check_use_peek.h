#include "app_cfg.h"
#ifndef __CHECK_USE_PEEK_H__
#define __CHECK_USE_PEEK_H__
#include "queue.h"
#include "check_string.h"
#include "../utilities/arm/app_type.h"
#include <stdbool.h>
#include <stdint.h>

typedef fsm_rt_t check_agent_handler_t(void *pTarget, read_byte_evt_handler_t *ptReadByte,  bool *pbRequestDrop);

typedef struct
{
    void *pTarget;
    check_agent_handler_t *fnCheckWords;
} check_agent_t;

typedef struct {
    uint8_t chState;
    uint8_t chAgentsNumber;
    uint8_t chVoteDropCount;
    uint8_t chWordsCount;
    byte_queue_t* pTarget;
    read_byte_evt_handler_t tReadByte;
    check_agent_t *ptAgents;
}check_use_peek_t;

typedef struct {
    uint8_t chAgentsNumber;
    byte_queue_t* pTarget;
    check_agent_t *ptAgents;
} check_use_peek_cfg_t;

extern bool check_use_peek_init(check_use_peek_t *ptThis, const check_use_peek_cfg_t *ptCFG);
extern fsm_rt_t task_check_use_peek(check_use_peek_t *ptThis);
#endif
