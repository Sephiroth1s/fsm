#include "app_cfg.h"
#ifndef __EVENT_H__
#define __EVENT_H__

#define SET_EVENT(__EVENT) set_event(__EVENT)
#define WAIT_EVENT(__EVENT) (wait_event(__EVENT))
#define RESET_EVENT(__EVENT) reset_event(__EVENT)
#define INIT_EVENT(__EVENT, __INIT_VALUE, __MANUAL) init_event(__EVENT, __INIT_VALUE, __MANUAL)

typedef struct {
    bool bAutoReset;
    bool bIsSet;
} event_t;

extern void set_event(event_t *ptEvent);
extern bool wait_event(event_t *ptEvent);
extern void reset_event(event_t *ptEvent);
extern void init_event(event_t *ptEvent, bool bInitValue, bool bManualReset);

#endif

