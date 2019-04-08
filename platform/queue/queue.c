
#include "app_cfg.h"
#include "queue.h"

bool wait_event(event_t *ptEvent) 
{
    if (ptEvent != NULL) {
        if (!ptEvent->bAutoReset) {
            if (ptEvent->bIsSet) {
                return true;
            }
        } else {
            if (ptEvent->bIsSet) {
                ptEvent->bIsSet = false;
                return true;
            }
        }
    }
    return false;
}
void init_event(event_t *ptEvent, bool bInitValue, bool bManualReset) 
{
    if (ptEvent != NULL) {
        ptEvent->bAutoReset = !bManualReset;
        ptEvent->bIsSet = bInitValue;
    }
}
void set_event(event_t *ptEvent) 
{
    if (ptEvent != NULL) {
        ptEvent->bIsSet = true;
    }
}
void reset_event(event_t *ptEvent) 
{
    if (ptEvent != NULL) {
        ptEvent->bIsSet = false;
    }
}
