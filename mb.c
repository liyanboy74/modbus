#include <stdio.h>
#include "mb.h"
#include "mb-check.h"
#include "mb-table.h"
#include "mb-process.h"
#include "mb-link.h"
#include "mb-packet.h"

mb_config_s MB_Config={.mode=MB_MODE_MASTER,.address=MB_DEFAULT_SLAVE_ADDRESS};

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

void mb_error_handler(mb_functions_e func,mb_error_e err)
{
    mb_link_prepare_tx_data(mb_packet_error(func,err));
}


void mb_rx_packet_handler(mb_packet_s Packet)
{
    mb_error_e err;

    err=mb_check_func(Packet.func);
    if(err){mb_error_handler(Packet.func,err);return;}

    if(mb_mode_get()==MB_MODE_SLAVE)
    {
        if(Packet.func==MB_FUNC_Read_Coils||Packet.func==MB_FUNC_Read_Discrete_Inputs)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            switch (Packet.func)
            {
                case MB_FUNC_Read_Coils:mb_slave_process_read_coils(Packet);break;
                case MB_FUNC_Read_Discrete_Inputs:mb_slave_process_read_discrere_inputs(Packet);break;
                default:break;
            }
        }
        else if(Packet.func==MB_FUNC_Read_Holding_Registers||Packet.func==MB_FUNC_Read_Input_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_table_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            switch (Packet.func)
            {
                case MB_FUNC_Read_Holding_Registers:mb_slave_process_read_holding_registers(Packet);break;
                case MB_FUNC_Read_Input_Registers:mb_slave_process_read_input_registers(Packet);break;
                default:break;
            }
        }
        else if(Packet.func==MB_FUNC_Write_Single_Coil)
        {
            err=mb_check_on_off(Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,0);
            if(err){mb_error_handler(Packet.func,err);return;}

            mb_slave_process_write_single_coil(Packet);
        }
        else if(Packet.func==MB_FUNC_Write_Single_Register)
        {
            err=mb_check_table_address(Packet.u16_1,0);
            if(err){mb_error_handler(Packet.func,err);return;}

            mb_slave_process_write_single_register(Packet);
        }
        else if(Packet.func==MB_FUNC_Write_Multiple_Coils)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_quantity_bit_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_table_bit_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            mb_slave_process_write_multiple_coils(Packet);
        }
        else if(Packet.func==MB_FUNC_Write_Multiple_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_quantity_reg_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(Packet.func,err);return;}

            err=mb_check_table_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(Packet.func,err);return;}

            mb_slave_process_write_multiple_register(Packet);
        }
    }
    else // Master
    {
        mb_master_process_packet(Packet);
    }
}

void mb_tx_packet_handler(mb_packet_s Packet)
{
    mb_link_prepare_tx_data(Packet);
}
