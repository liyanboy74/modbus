#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mb2.h"

int main()
{
    uint8_t Buffer[32],i,l=0;

    //l=mb_rtu_request_read_coil(Buffer,0x01,0x0000,0x0001);
    //l=mb_rtu_response_read_coil(Buffer,0x01,4,(uint8_t *)"ABCD");

    for(i=0;i<l;i++)printf("%02x ",Buffer[i]);
    return 0;
}