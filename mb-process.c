/*
 Process MODBUS Request
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stdio.h>
#include "mb-process.h"
#include "mb-table.h"
#include "mb-packet.h"

#if(MB_MODE==MB_MODE_SLAVE)

uint8_t MB_PROCESS_Buffer[MB_PROCESS_Buffer_Size];

#define MB_PROCESS_MAX_DATA (MB_PROCESS_Buffer_Size-2)

mb_error_e mb_slave_process_read_coils(mb_packet_s* Packet)
{
    uint16_t i,Start;
    uint8_t Size;
    uint32_t End;

    Start=MB_U16_AT(Packet->payload);
    End=(uint32_t)MB_U16_AT(Packet->payload+2)+Start;

    for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
    {
        if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
        MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Coils,Start)<<(i%8);
    }

    Size=(uint8_t)(i/8);
    if(i%8)Size++;

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_coil(Size,MB_PROCESS_Buffer));
    return MB_OK;
}

mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s* Packet)
{
    uint16_t i,Start;
    uint8_t Size;
    uint32_t End;

    Start=MB_U16_AT(Packet->payload);
    End=(uint32_t)MB_U16_AT(Packet->payload+2)+Start;

    for(i=0;Start<End&&(i/8)<MB_PROCESS_MAX_DATA;Start++,i++)
    {
        if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
        MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Discretes_Input,Start)<<(i%8);
    }

    Size=(uint8_t)(i/8);
    if(i%8)Size++;

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_discrete_inputs(Size,MB_PROCESS_Buffer));
    return MB_OK;
}

mb_error_e mb_slave_process_read_holding_registers(mb_packet_s* Packet)
{
    uint8_t i;
    uint16_t Start,Temp;
    uint32_t End;

    Start=MB_U16_AT(Packet->payload);
    End=(uint32_t)MB_U16_AT(Packet->payload+2)+Start;

    for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
    {
        Temp=mb_table_read(TABLE_Holding_Registers,Start);

        MB_PROCESS_Buffer[i]=(uint8_t)((Temp>>8)&0xff);
        MB_PROCESS_Buffer[i+1]=(uint8_t)(Temp&0xff);
    }

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_holding_registers(i,MB_PROCESS_Buffer));
    return MB_OK;
}

mb_error_e mb_slave_process_read_input_registers(mb_packet_s* Packet)
{
    uint8_t i;
    uint16_t Start,Temp;
    uint32_t End;

    Start=MB_U16_AT(Packet->payload);
    End=(uint32_t)MB_U16_AT(Packet->payload+2)+Start;

    for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
    {
        Temp=mb_table_read(TBALE_Input_Registers,Start);

        MB_PROCESS_Buffer[i]=(uint8_t)((Temp>>8)&0xff);
        MB_PROCESS_Buffer[i+1]=(uint8_t)(Temp&0xff);
    }

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_input_registers(i,MB_PROCESS_Buffer));
    return MB_OK;
}

mb_error_e mb_slave_process_write_single_coil(mb_packet_s* Packet)
{
    mb_table_write_bit(TBALE_Coils,MB_U16_AT(Packet->payload),Packet->payload[2]);
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_write_single_coil(MB_U16_AT(Packet->payload),MB_U16_AT(Packet->payload+2)));
    return MB_OK;
}

mb_error_e mb_slave_process_write_single_register(mb_packet_s* Packet)
{
    mb_table_write(TABLE_Holding_Registers,MB_U16_AT(Packet->payload),MB_U16_AT(Packet->payload+2));
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_write_single_register(MB_U16_AT(Packet->payload),MB_U16_AT(Packet->payload+2)));
    return MB_OK;
}

mb_error_e mb_slave_process_write_multiple_coils(mb_packet_s* Packet)
{
    uint16_t Start;
    uint16_t Quantity;
    uint32_t End;
    uint16_t i;

    Start    = MB_U16_AT(Packet->payload);
    Quantity = MB_U16_AT(Packet->payload + 2);

    End = (uint32_t)Start + Quantity;

    for(i = 0; Start < End; Start++, i++)
    {
        mb_table_write_bit(
            TBALE_Coils,
            Start,
            (Packet->payload[(i / 8) + 5] >> (i % 8)) & 0x01
        );
    }

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_write_multiple_coils(MB_U16_AT(Packet->payload),MB_U16_AT(Packet->payload+2)));
    return MB_OK;
}

mb_error_e mb_slave_process_write_multiple_register(mb_packet_s* Packet)
{
    uint16_t i, Start, Quantity, Temp;
    uint32_t End;

    Start    = MB_U16_AT(Packet->payload);
    Quantity = MB_U16_AT(Packet->payload + 2);
    End      = (uint32_t)Start + Quantity;

    for (i = 0; Start < End; Start++, i += 2)
    {
        Temp = MB_U16_AT(Packet->payload + 5 + i);
        mb_table_write(TABLE_Holding_Registers, Start, Temp);
    }

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_write_multiple_registers(MB_U16_AT(Packet->payload),MB_U16_AT(Packet->payload+2)));
    return MB_OK;
}

mb_error_e mb_slave_process_read_write_multiple_registers(mb_packet_s* Packet)
{
    uint8_t i;
    uint16_t Start, Quantity, Temp;
    uint32_t End;

    // Write
    Start    = MB_U16_AT(Packet->payload + 4);
    Quantity = MB_U16_AT(Packet->payload + 2 + 4);
    End      = (uint32_t)Start + Quantity;

    for (i = 0; Start < End; Start++, i += 2)
    {
        Temp = MB_U16_AT(Packet->payload + 5 + 4 + i);
        mb_table_write(TABLE_Holding_Registers, Start, Temp);
    }

    // Read
    Start=MB_U16_AT(Packet->payload);
    Quantity=MB_U16_AT(Packet->payload+2);
    End=(uint32_t) Start + Quantity;

    for(i=0;Start<End&&i<MB_PROCESS_MAX_DATA;Start++,i+=2)
    {
        Temp=mb_table_read(TABLE_Holding_Registers,Start);

        MB_PROCESS_Buffer[i]=(uint8_t)((Temp>>8)&0xff);
        MB_PROCESS_Buffer[i+1]=(uint8_t)(Temp&0xff);
    }

    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_write_multiple_registers(i,MB_PROCESS_Buffer));
    return MB_OK;
}

#if MB_ENABLE_FUNC_Encapsulated_Interface
mb_error_e mb_slave_process_read_device_identification(mb_packet_s* Packet)
{
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_device_identification(Packet->payload[2]));
    return MB_OK;
}
#endif

#if MB_ENABLE_FUNC_Read_Exception_Status
mb_error_e mb_slave_process_read_exeption_status(mb_packet_s* Packet)
{
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_read_exeption_status());
    return MB_OK;
}
#endif

#if MB_ENABLE_FUNC_Report_Server_ID
mb_error_e mb_slave_process_report_server_id(mb_packet_s* Packet)
{
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_response_report_server_id());
    return MB_OK;
}
#endif

#endif
