
#include "app_cfg.h"
#include "queue.h"

bool enqueue_byte( byte_queue_t* ptThis,uint8_t* pchByte)
{
     if(ptThis==NULL)
     {
         return false;
     }
     if(this.hwTail<this.hwSize){
        this.pchBuffer[this.hwTail]=*pchByte;
        this.hwTail++;
        return true;
     } 
     return false;
     
}
bool dequeue_byte(byte_queue_t* ptThis,uint8_t* pchByte)
{
    if(ptThis==NULL)
     {
         return false;
     }
     if(!is_byte_queue_empty(ptThis)){
        *pchByte=this.pchBuffer[this.hwHead];
        this.hwHead++;
        return true;
     } 
     return false;
}
bool is_byte_queue_empty(byte_queue_t* ptThis)
{
    if(ptThis != NULL)
    {
        if(!(this.hwTail-this.hwHead)){
            return true;
        }
    }
    return false;
}
bool init_byte_queue(byte_queue_t* ptThis,uint8_t *pchBuffer,uint16_t hwSize)
{
    if(ptThis!=NULL)
    {
        this.hwHead=0;
        this.hwTail=0;
        this.pchBuffer=pchBuffer;
        this.hwSize=hwSize;
        return true;
    }
    return false;
}