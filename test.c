#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>

#include "mb.h"
#include "fifo.h"
#include "mb-link.h"

void mb_master_process_packet(mb_packet_s Packet)
{
    #ifdef MB_DEBUG
    printf("M:OK\n");
    #endif
}

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

    uint8_t M1[]={0x11,0x04,0x02,0x00,0x0A,0xF8,0xF4};
    uint8_t M2[]={0x11,0x03,0x06,0xAE,0x41,0x56,0x52,0x43,0x40,0x49,0xAD};

    uint8_t S1[]={0x11,0x01,0x00,0x13,0x00,0x25,0x0E,0x84};
    uint8_t S2[]={0x11,0x03,0x00,0x6B,0x00,0x03,0x76,0x87};
    uint8_t S3[]={0x11,0x04,0x00,0x08,0x00,0x01,0xB2,0x98};
    uint8_t S4[]={0x11,0x05,0x00,0xAC,0xFF,0x00,0x4E,0x8B};

    uint8_t i,Byte;

    FIFO_Init(128);

    for(i=0;i<sizeof(S1);i++)
    {
        FIFO_Add(S1[i]);
    }
    for(i=0;i<sizeof(S2);i++)
    {
        FIFO_Add(S2[i]);
    }
    for(i=0;i<sizeof(S3);i++)
    {
        FIFO_Add(S3[i]);
    }
    for(i=0;i<sizeof(S4);i++)
    {
        FIFO_Add(S4[i]);
    }

    
    for(i=0;i<sizeof(M1);i++)
    {
        FIFO_Add(M1[i]);
    }
    for(i=0;i<sizeof(M2);i++)
    {
        FIFO_Add(M2[i]);
    }


    mb_mode_set(MB_MODE_SLAVE);

    for(i=0;i<sizeof(S1)+sizeof(S2)+sizeof(S3)+sizeof(S4);i++)
    {
        if(FIFO_Read(&Byte)==FIFO_OK)
        {
            mb_link_check_new_data(Byte);
        }
    }

    mb_mode_set(MB_MODE_MASTER);

    for(i=0;i<(sizeof(M1)+sizeof(M2));i++)
    {
        if(FIFO_Read(&Byte)==FIFO_OK)
        {
            mb_link_check_new_data(Byte);
        }
    }

    return 0;
}