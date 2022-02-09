#include "mb-process.h"
#include "mb-table.h"
#include "mb-link.h"
#include "mb-packet.h"

uint8_t MB_PROCESS_Buffer[MB_PROCESS_Buffer_Size];
uint8_t MB_PROCESS_Buffer_Index=0;

mb_packet_s MB_PROCESS_Responce;

mb_error_e mb_slave_process_read_coils(mb_packet_s Packet)
{
    uint16_t i,Start,Size;
    uint32_t End;

    Start=Packet.u16_1;
    End=Packet.u16_2+Start;

    for(i=0;Start<End;Start++,i++)
    {
        if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
        MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Coils,Start)<<(i%8);
    }

    Size=i/8;
    if(i%8)Size++;

    return mb_link_prepare_tx_data(mb_packet_response_read_coil(Size,MB_PROCESS_Buffer));
}

mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s Packet)
{
    uint16_t i,Start,Size;
    uint32_t End;

    Start=Packet.u16_1;
    End=Packet.u16_2+Start;

    for(i=0;Start<End;Start++,i++)
    {
        if(i%8==0)MB_PROCESS_Buffer[i/8]=0;
        MB_PROCESS_Buffer[i/8]|=mb_table_read_bit(TBALE_Discretes_Input,Start)<<(i%8);
    }

    Size=i/8;
    if(i%8)Size++;

    return mb_link_prepare_tx_data(mb_packet_response_read_discrete_inputs(Size,MB_PROCESS_Buffer));
}

mb_error_e mb_slave_process_read_holding_registers(mb_packet_s Packet)
{
    uint16_t i,Start,Temp;
    uint32_t End;

    Start=Packet.u16_1;
    End=Packet.u16_2+Start;

    for(i=0;Start<End;Start++,i+=2)
    {
        Temp=mb_table_read(TABLE_Holding_Registers,Start);

        MB_PROCESS_Buffer[i]=(Temp>>8)&0xff;
        MB_PROCESS_Buffer[i+1]=Temp&0xff;
    }

    return mb_link_prepare_tx_data(mb_packet_response_read_holding_registers(i,MB_PROCESS_Buffer));
}

mb_error_e mb_slave_process_read_input_registers(mb_packet_s Packet)
{
    uint16_t i,Start,Temp;
    uint32_t End;

    Start=Packet.u16_1;
    End=Packet.u16_2+Start;

    for(i=0;Start<End;Start++,i+=2)
    {
        Temp=mb_table_read(TBALE_Input_Registers,Start);

        MB_PROCESS_Buffer[i]=(Temp>>8)&0xff;
        MB_PROCESS_Buffer[i+1]=Temp&0xff;
    }

    return mb_link_prepare_tx_data(mb_packet_response_read_input_registers(i,MB_PROCESS_Buffer));
}

mb_error_e mb_slave_process_write_single_coil(mb_packet_s Packet)
{
    return MB_ERROR_FAILED_TO_RESPOND;
}

mb_error_e mb_slave_process_write_single_register(mb_packet_s Packet)
{
    return MB_ERROR_FAILED_TO_RESPOND;
}

mb_error_e mb_slave_process_write_multiple_coils(mb_packet_s Packet)
{
    return MB_ERROR_FAILED_TO_RESPOND;
}

mb_error_e mb_slave_process_write_multiple_register(mb_packet_s Packet)
{
    return MB_ERROR_FAILED_TO_RESPOND;
}
