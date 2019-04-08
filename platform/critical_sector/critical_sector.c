
#include "app_cfg.h"
#include "critical_sector.h"

void init_critical_sector(critical_sector_t *ptCritical) 
{
    if (ptCritical != NULL) {
        ptCritical->bLocked = false;
    }
}
bool enter_critical_sector(critical_sector_t *ptCritical) 
{
    if (ptCritical != NULL) {
        if (!ptCritical->bLocked) {
            ptCritical->bLocked = true;
            return true;
        }
    }
    return false;
}
void leave_critical_sector(critical_sector_t *ptCritical) 
{
    if (ptCritical != NULL) {
        ptCritical->bLocked = false;
    }
}
