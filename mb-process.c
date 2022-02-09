#include "mb-process.h"
#include "mb-table.h"
#include "mb-link.h"

uint8_t MB_PROCESS_Buffer[MB_PROCESS_Buffer_Size];
uint8_t MB_PROCESS_Buffer_Index=0;

mb_packet_s MB_PROCESS_Responce;

mb_error_e mb_slave_process_read_coils(mb_packet_s Packet)
{
    uint16_t i,Start,Size;

    Start=Packet.u16_1;
    Size=Packet.u16_2;

    for(i=0;Start<Size;Start++,i++)
    {
        if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
        MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Coils,Start)<<(i%8);
    }

    MB_PROCESS_Responce.len=i/8;
    if(i%8)MB_PROCESS_Responce.len++;

    MB_PROCESS_Responce.Data=MB_PROCESS_Buffer;

    MB_PROCESS_Responce.func=Packet.func;
    MB_PROCESS_Responce.device_address=mb_slave_address_get();

    return mb_link_prepare_tx_data(MB_PROCESS_Responce);
}

mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_read_holding_registers(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_read_input_registers(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_write_single_coil(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_write_single_register(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_write_multiple_coils(mb_packet_s Packet)
{
    return MB_OK;
}

mb_error_e mb_slave_process_write_multiple_register(mb_packet_s Packet)
{
    return MB_OK;
}
