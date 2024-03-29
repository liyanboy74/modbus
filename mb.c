/*
 MODBUS
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stdio.h>
#include "mb.h"
#include "mb-check.h"
#include "mb-table.h"
#include "mb-process.h"
#include "mb-link.h"
#include "mb-packet.h"

mb_config_s MB_Config={MB_DEFAULT_SLAVE_ADDRESS};

#if(MB_MODE==MB_MODE_SLAVE)

void mb_slave_address_set(uint8_t Address)
{
    MB_Config.address=Address;
}

uint8_t mb_slave_address_get(void)
{
    return MB_Config.address;
}

#elif(MB_MODE==MB_MODE_MASTER)

void mb_set_master_process_handler(void (*f)(mb_packet_s))
{
    MB_Config.master_process_handler=f;
}

#endif

void mb_set_tx_handler(void (*f)(uint8_t *,uint8_t))
{
    MB_Config.tx_handler=f;
}

#if(MB_MODE==MB_MODE_SLAVE)
void mb_error_handler(mb_packet_s* Packet,mb_error_e err)
{
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->device_address != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_error(Packet->func,err));
}
#endif

void mb_rx_packet_handler(mb_packet_s Packet)
{
    #if(MB_MODE==MB_MODE_SLAVE)

        mb_error_e err;

        err=mb_check_func(Packet.func);
        if(err){mb_error_handler(&Packet,err);return;}

        #if MB_ENABLE_FUNC_Read_Coils
        if(Packet.func==MB_FUNC_Read_Coils)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_coils(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Read_Discrete_Inputs
        if(Packet.func==MB_FUNC_Read_Discrete_Inputs)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_discretes_input_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_discrere_inputs(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Read_Holding_Registers
        if(Packet.func==MB_FUNC_Read_Holding_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_holding_registers(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Read_Input_Registers
        if(Packet.func==MB_FUNC_Read_Input_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_input_registers_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_input_registers(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Write_Single_Coil
        if(Packet.func==MB_FUNC_Write_Single_Coil)
        {
            err=mb_check_on_off(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(Packet.u16_1,0);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_single_coil(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Write_Single_Register
        if(Packet.func==MB_FUNC_Write_Single_Register)
        {
            err=mb_check_table_holding_registers_address(Packet.u16_1,0);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_single_register(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Write_Multiple_Coils
        if(Packet.func==MB_FUNC_Write_Multiple_Coils)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_bit_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_multiple_coils(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif
        #if MB_ENABLE_FUNC_Write_Multiple_Registers
        if(Packet.func==MB_FUNC_Write_Multiple_Registers)
        {
            err=mb_check_quantity(Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_reg_n_byte(Packet.u16_2,Packet.len);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(Packet.u16_1,Packet.u16_2);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_multiple_register(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        } else
        #endif
        return;

    #elif(MB_MODE==MB_MODE_MASTER)

        if(MB_Config.master_process_handler!=NULL)
        MB_Config.master_process_handler(Packet);

    #endif
}

void mb_tx_packet_handler(mb_packet_s Packet)
{
    mb_link_prepare_tx_data(Packet);
}

void mb_rx_new_data(uint8_t Byte)
{
    mb_link_check_new_data(Byte);
}

void mb_rx_timeout_handler(void)
{
    mb_link_reset_rx_buffer();
}
