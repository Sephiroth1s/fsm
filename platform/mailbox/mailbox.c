
#include "app_cfg.h"
#include "mailbox.h"
#include "event.h"
void init_mail(mailbox_t *ptEcho) 
{
    if (ptEcho != NULL) {
        INIT_EVENT(&ptEcho->tSealed, false, false);
        ptEcho->pTarget = NULL;
    }
}
void post_mail(mailbox_t *ptEcho, void *pTarget) 
{
    if (ptEcho != NULL) {
        SET_EVENT(&ptEcho->tSealed);
        ptEcho->pTarget = pTarget;
    }
}
void *open_mail(mailbox_t *ptEcho) 
{
    if (ptEcho != NULL) {
        if (WAIT_EVENT(&ptEcho->tSealed)) {
            return ptEcho->pTarget;
        }
    }
    return NULL;
}