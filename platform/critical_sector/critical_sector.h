#include "app_cfg.h"
#ifndef __CRITICAL_SECTOR_H__
#define __CRITICAL_SECTOR_H__

#include <stdbool.h>

#define ENTER_CRITICAL_SECTOR(__CRITICAL) (enter_critical_sector(__CRITICAL))
#define LEAVE_CRITICAL_SECTOR(__CRITICAL) leave_critical_sector(__CRITICAL)
#define INIT_CRITICAL_SECTOR(__CRITICAL) init_critical_sector(__CRITICAL)

typedef struct {
    bool bLocked;
} critical_sector_t;
typedef critical_sector_t mutex_t;

extern void init_critical_sector(critical_sector_t *ptCritical);
extern bool enter_critical_sector(critical_sector_t *ptCritical);
extern void leave_critical_sector(critical_sector_t *ptCritical); 
#endif
