#include "app_cfg.h"
#ifndef __CHECK_USE_PEEK_H__
#define __CHECK_USE_PEEK_H__
#include "queue.h"
#include "../utilities/arm/app_type.h"
#include <stdbool.h>
#include <stdint.h>
#define this (*ptThis)
typedef fsm_rt_t check_words_agent_t(byte_queue_t*,bool*);
typedef struct 
{
    uint8_t chState;
    uint8_t chWordsNumber;
    byte_queue_t *pTarget;
    check_words_agent_t **fnCheckWords;
}check_words_t;

typedef struct 
{
    uint8_t chWordsNumber;
    byte_queue_t *pTarget;
    check_words_agent_t **fnCheckWords;
}check_words_cfg_t;

extern bool check_use_peek_init(check_words_t *ptThis, const check_words_cfg_t *ptCFG);
extern fsm_rt_t task_check_use_peek(check_words_t *ptThis);
#endif
