#include "app_cfg.h"
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define ENQUEUE_BYTE(__QUEUE,__OBJ) (enqueue_byte(__QUEUE,__OBJ))
#define DEQUEUE_BYTE(__QUEUE,__ADDR) (dequeue_byte(__QUEUE,__ADDR))
#define IS_BYTE_QUEUE_EMPTY(__QUEUE) (is_byte_queue_empty(__QUEUE))
#define INIT_BYTE_QUEUE(__QUEUE, __BUFFER, __SIZE) (init_byte_queue(__QUEUE, __BUFFER, __SIZE))

typedef struct {
    uint8_t *pchBuffer;
    uint16_t hwSize;
    uint16_t hwHead;
    uint16_t hwTail;
    uint16_t hwLength;
} byte_queue_t;

extern bool enqueue_byte(byte_queue_t* ptThis, uint8_t pchByte);
extern bool dequeue_byte(byte_queue_t* ptThis, uint8_t* pchByte);
extern bool is_byte_queue_empty(byte_queue_t* ptThis);
extern bool is_byte_queue_full(byte_queue_t* ptThis);
extern bool init_byte_queue(byte_queue_t* ptThis, uint8_t* pchByte,uint16_t hwSize);

#endif

