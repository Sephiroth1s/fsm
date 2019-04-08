#include "app_cfg.h"
#ifndef __QUEUE_H__
#define __QUEUE_H__
#define TASK_RESET_FSM()  \
    do {                  \
        s_tState = START; \
    } while (0)

#define ENQUEUE_BYTE(__QUEUE, __OBJ) //入队操作
#define DEQUEUE_BYTE(__QUEUE, __ADDR) //出队操作
#define IS_BYTE_QUEUE_EMPTY(__QUEUE) //判断队列是否为空
#define INIT_BYTE_QUEUE(__QUEUE, __BUFFER, __SIZE) //初始化队列

typedef struct {
    uint8_t *pchBuffer;
    uint16_t hwSize;
    uint16_t hwHead;
    uint16_t hwTail;
    uint16_t hwLength;
} byte_queue_t;

extern enqueue_byte( byte_queue_t* ptThis);
extern dequeue_byte(byte_queue_t* ptThis);
extern bool is_byte_queue_empty(byte_queue_t* ptThis);
extern init_byte_queue(byte_queue_t* ptThis,uint16_t hwBuffer,uint16_t hwSize);

#endif

