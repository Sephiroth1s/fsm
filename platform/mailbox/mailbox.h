#include "app_cfg.h"
#ifndef __MAILBOX_H__
#define __MAILBOX_H__
#include "event.h"
#include <stdbool.h>
#include <stdint.h>
    
#define INIT_MAIL(__MAIL) init_mail(__MAIL)
#define POST_MAIL(__MAIL, __TARGET) post_mail(__MAIL, __TARGET)
#define OPEN_MAIL(__MAIL) (open_mail(__MAIL))

typedef struct {
    event_t tSealed;
    void *pTarget;
} mailbox_t;

extern void init_mail(mailbox_t *ptEcho);
extern void post_mail(mailbox_t *ptEcho, void *pTarget);
extern void *open_mail(mailbox_t *ptEcho);

#endif
