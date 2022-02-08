#include <stdio.h>
#include "mb.h"
#include "mb-check.h"
#include "mb-table.h"

mb_config_s MB_Config={.mode=MB_MODE_MASTER,.address=0x11};

void mb_mode_set(mb_mode_e Mode)
{
    MB_Config.mode=Mode;
}

mb_mode_e mb_mode_get(void)
{
    return MB_Config.mode;
}

void mb_slave_address_set(uint8_t Address)
{
    MB_Config.address=Address;
}

uint8_t mb_slave_address_get(void)
{
    return MB_Config.address;
}

mb_packet_s mb_rx_packet_split(uint8_t *Packet_Buffer,uint8_t Packet_Len,mb_packet_type_e Packet_Type)
{
    uint8_t i;
    mb_packet_s Packet;

    Packet.device_address=Packet_Buffer[0];
    Packet.func=Packet_Buffer[1];
    Packet.type=Packet_Type;

    if(Packet.type==MB_PACKET_Master_Responce_Var)
    {
        Packet.len=Packet_Buffer[2];
        Packet.Data=&Packet_Buffer[3];
    }
    else if(Packet.type==MB_PACKET_Master_Responce_Fix||Packet.type==MB_PACKET_Slave_Responce_Fix)
    {
        Packet.u16_1=(Packet_Buffer[2]<<8)|(Packet_Buffer[3]);
        Packet.u16_2=(Packet_Buffer[4]<<8)|(Packet_Buffer[5]);
    }
    else if(Packet.type==MB_PACKET_Slave_Responce_Var)
    {
        Packet.u16_1=(Packet_Buffer[2]<<8)|(Packet_Buffer[3]);
        Packet.u16_2=(Packet_Buffer[4]<<8)|(Packet_Buffer[5]);
        Packet.len=Packet_Buffer[6];
        Packet.Data=&Packet_Buffer[7];
    }

    return Packet;
}

void mb_error_handler(mb_error_e err)
{
    #ifdef MB_DEBUG
    printf("MB_ERROR: %d\n",err);
    #endif
    return;
}

mb_error_e mb_slave_process_read_coils(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_read_holding_registers(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_read_input_registers(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_write_single_coil(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_write_single_register(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_write_multiple_coils(mb_packet_s Packet)
{

}

mb_error_e mb_slave_process_write_multiple_register(mb_packet_s Packet)
{

}


void mb_rx_packet_handler(mb_packet_s Packet)
{
    mb_error_e err;

    err=mb_check_func(Packet.func);
    if(err){mb_error_handler(err);return;}

    if(mb_mode_get()==MB_MODE_SLAVE)
    {
        if(Packet.func==MB_FUNC_Read_Coils||Packet.func==MB_FUNC_Read_Discrete_Inputs)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            switch (Packet.func)
            {
                case MB_FUNC_Read_Coils:err=mb_slave_process_read_coils(Packet);break;
                case MB_FUNC_Read_Discrete_Inputs:err=mb_slave_process_read_discrere_inputs(Packet);break;
                default:break;
            }

            if(err){mb_error_handler(err);return;}
        }
        else if(Packet.func==MB_FUNC_Read_Holding_Registers||Packet.func==MB_FUNC_Read_Input_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_check_table_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            switch (Packet.func)
            {
                case MB_FUNC_Read_Holding_Registers:err=mb_slave_process_read_holding_registers(Packet);break;
                case MB_FUNC_Read_Input_Registers:err=mb_slave_process_read_input_registers(Packet);break;
                default:break;
            }

            if(err){mb_error_handler(err);return;}
        }
        else if(Packet.func==MB_FUNC_Write_Single_Coil)
        {
            err=mb_check_on_off(Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,0);
            if(err){mb_error_handler(err);return;}

            err=mb_slave_process_write_single_coil(Packet);
            if(err){mb_error_handler(err);return;}
        }
        else if(Packet.func==MB_FUNC_Write_Single_Register)
        {
            err=mb_check_table_address(Packet.u16_1,0);
            if(err){mb_error_handler(err);return;}

            err=mb_slave_process_write_single_register(Packet);
            if(err){mb_error_handler(err);return;}
        }
        else if(Packet.func==MB_FUNC_Write_Multiple_Coils)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_check_quantity_bit_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_slave_process_write_multiple_coils(Packet);
            if(err){mb_error_handler(err);return;}
        }
        else if(Packet.func==MB_FUNC_Write_Multiple_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_check_quantity_reg_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(err);return;}

            err=mb_check_table_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(err);return;}

            err=mb_slave_process_write_multiple_register(Packet);
            if(err){mb_error_handler(err);return;}
        }
    }
    else // Master
    {

    }


}

