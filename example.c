#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mb2.h"
#include "table.h"

int main()
{
    uint8_t Buffer[32],i,l=0;
    uint16_t Temp;

    //l=mb_rtu_request_read_coil(Buffer,0x11,0x0013,0x0025);
    //l=mb_rtu_response_read_coil(Buffer,0x11,5,(uint8_t *)"ABCDE");
    //for(i=0;i<l;i++)printf("%02x ",Buffer[i]);

    TABLE_Write(TBALE_Coils,0,0x1234);
    TABLE_Write(TBALE_Coils,1,0x5678);

    Temp=TABLE_Read(TBALE_Coils,1);

    printf("Data: %x\r\n",Temp);

    return 0;
}