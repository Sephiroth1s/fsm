#include "app_cfg.h"
#include "queue.h"
#include "check_string.h"
#include "check_use_peek.h"
#include "../utilities/arm/app_type.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define this (*ptThis)
#define TASK_RESET_FSM()      \
    do {                      \
        this.chState = START; \
    } while (0);

bool check_use_peek_init(check_use_peek_t *ptThis, const check_use_peek_cfg_t *ptCFG)
{
    enum {
        START
    };
    if ((NULL == ptThis) || (NULL == ptCFG) || (NULL == ptCFG->ptAgents) || (NULL == ptCFG->ptAgents->pTarget) || (NULL == ptCFG->ptAgents->fnCheckWords)) {
        return false;
    }
    this.chState = START;
    this.chAgentsNumber = ptCFG->chAgentsNumber;
    this.pTarget = ptCFG->pTarget;
    this.ptAgents = ptCFG->ptAgents;
    return true;
}

fsm_rt_t task_check_use_peek(check_use_peek_t *ptThis)
{
    enum {
        START,
        CHECK_WORDS,
        DROP,
        CHECK_WORDS_NUMBER
    };
    read_byte_evt_handler_t tReadByte ={peek_byte_queue,this.pTarget};
    uint8_t chVoteDropCount;
    uint8_t chWordsCount;
    bool bIsRequestDrop;
    uint8_t chByteDrop;
    switch (this.chState) {
        case START:
            chVoteDropCount = 0;
            chWordsCount = 0;
            bIsRequestDrop = false;
            this.chState = CHECK_WORDS;
            // break;
        case CHECK_WORDS:
        GOTO_CHECK_WORDS:
            RESET_PEEK_BYTE(this.pTarget);
            if (fsm_rt_cpl == this.ptAgents[chWordsCount].fnCheckWords(this.ptAgents[chWordsCount].pTarget, &tReadByte, &bIsRequestDrop)) {
                GET_ALL_PEEKED_BYTE(this.pTarget);
                TASK_RESET_FSM();
                return fsm_rt_cpl;
            }
            if (bIsRequestDrop) {
                chVoteDropCount++;
            }
            bIsRequestDrop = false;
            this.chState = DROP;
            //break;
        case DROP:
            if (chVoteDropCount >= this.chAgentsNumber) {
                DEQUEUE_BYTE(this.pTarget, &chByteDrop);
                RESET_PEEK_BYTE(this.pTarget);
                chVoteDropCount = 0;
            }
            this.chState = CHECK_WORDS_NUMBER;
            //break;
        case CHECK_WORDS_NUMBER:
            chWordsCount++;
            if (chWordsCount >= this.chAgentsNumber) {
                chWordsCount = 0;
                TASK_RESET_FSM();
                break;
            }
            goto GOTO_CHECK_WORDS;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
