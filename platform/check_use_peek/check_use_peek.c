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
    if (   (NULL == ptCFG) 
        || (NULL == ptThis) 
        || (NULL == ptCFG->ptQueue)
        || (NULL == ptCFG->ptAgents) 
        || (NULL == ptCFG->ptAgents->pTarget) 
        || (NULL == ptCFG->ptAgents->fnCheckWords)) {
        return false;
    }
    this.chState = START;
    this.chAgentsNumber = ptCFG->chAgentsNumber;
    this.ptQueue = ptCFG->ptQueue;
    this.ptAgents = ptCFG->ptAgents;
    this.tReadByte.fnReadByte=(read_byte_t*)peek_byte_queue;
    this.tReadByte.pTarget=ptCFG->ptQueue;
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
    switch (this.chState) {
        case START:
            this.chVoteDropCount = 0;
            this.chWordsCount = 0;
            this.chState = CHECK_WORDS;
            // break;
        case CHECK_WORDS:
            do {
                bool bIsRequestDrop = false;
                RESET_PEEK_BYTE(this.ptQueue);
                if (fsm_rt_cpl == this.ptAgents[this.chWordsCount].fnCheckWords(this.ptAgents[this.chWordsCount].pTarget,&this.tReadByte, &bIsRequestDrop)) {
                    GET_ALL_PEEKED_BYTE(this.ptQueue);
                    TASK_RESET_FSM();
                    return fsm_rt_cpl;
                }
                if (bIsRequestDrop) {
                    this.chVoteDropCount++;
                }
                bIsRequestDrop = false;
            } while (0);
            this.chState = CHECK_WORDS_NUMBER;
            //break;
        case CHECK_WORDS_NUMBER:
            this.chWordsCount++;
            if (this.chWordsCount >= this.chAgentsNumber) {
                this.chWordsCount = 0;
                this.chState = DROP;
                //break;
            } else{
                this.chState = CHECK_WORDS;
                break;
            }
        case DROP:
            if (this.chVoteDropCount >= this.chAgentsNumber) {
                do {
                    uint8_t chByteDrop;
                    DEQUEUE_BYTE(this.ptQueue, &chByteDrop);
                } while (0);
                RESET_PEEK_BYTE(this.ptQueue);
                this.chVoteDropCount = 0;
            }
            TASK_RESET_FSM();
            break;
        default:
            return fsm_rt_err;
            break;
    }
    return fsm_rt_on_going;
}
