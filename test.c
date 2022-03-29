/*
 Test for MODBUS
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stdio.h>
#include <string.h>

#include "mb.h"

#include "mb-crc.h"
#include "mb-packet.h"

#define TestNum 10

int AnsIndex=0,AnsPIndex=0;

void send_data_to_mb(uint8_t * Data,uint8_t Size)
{
    int i;
    for(i=0;i<Size;i++)
    {
        mb_rx_new_data(Data[i]);
    }
}

const uint8_t SlaveAns[TestNum][9]={
    {0x01,0x05,0x00,0x00,0xff,0x00,0x8c,0x3a},
    {0x01,0x01,0x02,0x01,0x00,0xb8,0x6c},
    {0x01,0x06,0x00,0x00,0xff,0xff,0x88,0x7a},
    {0x01,0x03,0x02,0xff,0xff,0xb9,0xf4},
    {0x01,0x02,0x02,0x00,0x00,0xb9,0xb8},
    {0x01,0x04,0x02,0x00,0x00,0xb9,0x30},
    {0x01,0x0f,0x00,0x00,0x00,0x10,0x54,0x07},
    {0x01,0x01,0x02,0x00,0xff,0xf9,0xbc},
    {0x01,0x10,0x00,0x01,0x00,0x01,0x50,0x09},
    {0x01,0x03,0x02,0x00,0xff,0xf8,0x04},
};

const uint8_t MasterReq[TestNum][11]={
    {0x01,0x05,0x00,0x00,0xff,0x00,0x8c,0x3a},
    {0x01,0x01,0x00,0x00,0x00,0x10,0x3d,0xc6},
    {0x01,0x06,0x00,0x00,0xff,0xff,0x88,0x7a},
    {0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0a},
    {0x01,0x02,0x00,0x00,0x00,0x10,0x79,0xc6},
    {0x01,0x04,0x00,0x00,0x00,0x01,0x31,0xca},
    {0x01,0x0f,0x00,0x00,0x00,0x10,0x02,0x00,0xff,0xa2,0x60},
    {0x01,0x01,0x00,0x00,0x00,0x10,0x3d,0xc6},
    {0x00,0x10,0x00,0x01,0x00,0x01,0x02,0x00,0xff,0xea,0x51},   // Broadcast don't answer
    {0x01,0x03,0x00,0x01,0x00,0x01,0xd5,0xca},
};

void bar()
{
    int i;
    for(i=0;i<50;i++)putchar('=');
    printf("\n");
}

#if(MB_MODE==MB_MODE_MASTER)
int MPIndex=0;
void master_process(mb_packet_s Packet)
{
    int i;
    printf("MP: %02x %02x ",Packet.device_address,Packet.func);
    if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Var)
    {
        printf("%02x ",Packet.len);
        for(i=0;i<Packet.len;i++)
        {
            printf("%02x",Packet.Data[i]);
        }
        printf("\n");
    }
    else if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Fix)
    {
        printf("%04x %04x\n",Packet.u16_1,Packet.u16_2);
    }
    else if(Packet.type==MB_PACKET_TYPE_ERROR)
    {
        printf("ERROR CODE %02x in %02x\n",Packet.err,Packet.func&0x7f);
    }
    MPIndex++;
}

void send_data(uint8_t *Data,uint8_t Len)
{
    int i;
    printf("TX: ");
    for(i=0;i<Len;i++)printf("%02x ",Data[i]);
    if(strncmp((char*)Data,(char*)MasterReq[AnsIndex++],Len)==0)
    {
        printf("\t\tOK");
        AnsPIndex++;
    }
    else printf("\t\tERROR");
    printf("\n");
}

#else

void send_data(uint8_t *Data,uint8_t Len)
{
    int i;
    printf("TX: ");
    for(i=0;i<Len;i++)printf("%02x ",Data[i]);
    if(AnsIndex==8)AnsIndex++; // Break Broadcast packet
    if(strncmp((char*)Data,(char*)SlaveAns[AnsIndex++],Len)==0)
    {
        printf("\t\tOK");
        AnsPIndex++;
    }
    else printf("\t\tERROR");
    printf("\n");
}

#endif


int main()
{
    int i,j;

    //Set Handler for transmit data from MODBUS layer
    mb_set_tx_handler(&send_data);

    bar();

    #if(MB_MODE==MB_MODE_SLAVE)

    printf("Simulate receive packets from master:\n\n");


    for(i=0;i<TestNum;i++)
    {
        for(j=0;j<11;j++)
        {
            mb_rx_new_data(MasterReq[i][j]);
        }
        mb_rx_timeout_handler();
    }

    printf("\nSTATUS %02d/%02d %s\n",AnsPIndex+1,TestNum,(TestNum==(AnsPIndex+1))?"PASS":"FAIL");

    #elif(MB_MODE==MB_MODE_MASTER)

    //Set handler for process received packet in master mode
    mb_set_master_process_handler(&master_process);

    uint8_t TData[]={0x00,0xff};

    mb_packet_s Packets[]={
        mb_packet_request_write_single_coil(1,0,MB_COIL_ON),
        mb_packet_request_read_coil(1,0,16),
        mb_packet_request_write_single_register(1,0,0xffff),
        mb_packet_request_read_holding_registers(1,0,1),
        mb_packet_request_read_discrete_inputs(1,0,16),
        mb_packet_request_read_input_registers(1,0,1),
        mb_packet_request_write_multiple_coils(1,0,16,2,TData),
        mb_packet_request_read_coil(1,0,16),
        mb_packet_request_write_multiple_registers(0,1,1,2,TData),
        mb_packet_request_read_holding_registers(1,1,1)
    };

    printf("Test generate and send packets to slave:\n\n");

    for(i=0;i<sizeof(Packets)/sizeof(mb_packet_s);i++)
    mb_tx_packet_handler(Packets[i]);

    printf("\nSTATUS %02d/%02d %s\n",AnsPIndex,TestNum,(TestNum==AnsPIndex)?"PASS":"FAIL");

    bar();
    printf("Process received packet from slave:\n\n");

    for(i=0;i<TestNum;i++)
    {
        for(j=0;j<9;j++)
        {
            mb_rx_new_data(SlaveAns[i][j]);
        }
        mb_rx_timeout_handler();
    }

    printf("\nSTATUS %02d/%02d %s\n",MPIndex,TestNum,(TestNum==MPIndex)?"PASS":"FAIL");

    #endif

    bar();

    return 0;
}
