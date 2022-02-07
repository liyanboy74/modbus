#include <stdint.h>
#include "mb.h"
#include "mb-crc.h"

#include "fifo.h"

#define MB_LINK_Bffer_Size 128

uint8_t MB_LINK_Buffer[MB_LINK_Bffer_Size];
uint8_t MB_LINK_BufferIndex=0;

void mb_link_check_new_data(void)
{
    uint8_t Byte;
    static uint8_t ByteC=0;
    if(FIFO_Read(&Byte)==FIFO_OK)
    {
        
    }
}



