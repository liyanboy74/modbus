/*
 MODBUS TCP Layer
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stddef.h>
#include "mb-tcp.h"

static uint16_t transaction_id = 0;
uint8_t MB_TCP_Tx_Buffer[MB_TCP_Tx_Buffer_Size];

void mb_tcp_send(uint8_t *Data, uint16_t Len)
{
    if(mb.tx_handler!=NULL)
    mb.tx_handler(Data,Len);
}

// Detect MBAP Header
void mb_tcp_receive(uint8_t *Data, uint16_t Len)
{
    mb_packet_s Packet;
    uint16_t pdu_len;

    if(Len < 8) return;

    // Protocol ID
    if(Data[2] != 0 || Data[3] != 0)return;

    // Length
    pdu_len = ((uint16_t)Data[4] << 8) | Data[5];

    // Check Len
    if((pdu_len + 6) != Len) return;

    transaction_id   =Data[0]<<8;
    transaction_id  |=Data[1];

    Packet.unit_id  = Data[6];
    
    #if(MB_MODE==MB_MODE_SLAVE)
        mb.address=Packet.unit_id;
    #endif

    Packet.function = Data[7];
    Packet.length = pdu_len - 2;
    Packet.payload=&Data[8];

    mb_rx_packet_handler(Packet);
}

void mb_tcp_prepare_tx_data(mb_packet_s Packet)
{
    uint16_t len;

    #if(MB_MODE==MB_MODE_MASTER)
        transaction_id++;
        MB_TCP_Tx_Buffer[6] = Packet.unit_id;
    #else
        MB_TCP_Tx_Buffer[6] = mb.address;
    #endif

    MB_TCP_Tx_Buffer[0] = (uint8_t)(transaction_id >> 8);
    MB_TCP_Tx_Buffer[1] = (uint8_t)(transaction_id & 0xff);
    MB_TCP_Tx_Buffer[2] = 0;
    MB_TCP_Tx_Buffer[3] = 0;

    len = Packet.length + 2; // Unit ID + Function + Data

    MB_TCP_Tx_Buffer[4] = (uint8_t)(len >> 8);
    MB_TCP_Tx_Buffer[5] = (uint8_t)(len & 0xff);

    MB_TCP_Tx_Buffer[7] = Packet.function;

    for(uint16_t i=0; i<Packet.length && (i+8)<MB_TCP_Tx_Buffer_Size ;i++)
    {
        MB_TCP_Tx_Buffer[8+i] = Packet.payload[i];
    }

    mb_tcp_send(MB_TCP_Tx_Buffer, len+6);
}

// TCP Transport
void mb_tx_packet_handler(mb_packet_s Packet)
{
    mb_tcp_prepare_tx_data(Packet);
}
