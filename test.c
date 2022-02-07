#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mb-crc.h"

int main()
{
    /*
    mb_error_e error;
    error=mb_check_coils_address(15,1);
    printf("Error Code %02x\r\n",error);
    */
    /*
    mb_table_write(TBALE_Coils,0,0xf0f2);
    printf("%04x \r\n",mb_table_read(TBALE_Coils,0));
    printf("%02x \r\n",mb_table_read_bit(TBALE_Coils,1));
    */

   /*
    uint8_t Packet[8]={0x11,0x01,0x00,0x13,0x00,0x25};//,0x0e,0x84};
    mb_add_crc(Packet,6);
    printf("crc = %d\n%04x",mb_check_crc(Packet,8),*(uint16_t*)&Packet[6]);
    */

    return 0;
}