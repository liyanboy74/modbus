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
#include "mb-packet.h"

mb_config_s mb={MB_DEFAULT_SLAVE_ADDRESS,0,0,0,0};

#if(MB_MODE==MB_MODE_SLAVE)

void mb_slave_address_set(uint8_t Address)
{
    mb.address=Address;
}

uint8_t mb_slave_address_get(void)
{
    return mb.address;
}

#if MB_ENABLE_FUNC_Read_Exception_Status
void mb_slave_status_set(uint8_t status)
{
    mb.status=status;
}
uint8_t mb_slave_status_get(void)
{
    return mb.status;
}
#endif

#if MB_ENABLE_FUNC_Report_Server_ID
void mb_slave_set_server_id(uint8_t id)
{
    mb.server_id=id;
}
#endif

#elif(MB_MODE==MB_MODE_MASTER)

void mb_set_master_process_handler(void (*f)(mb_packet_s))
{
    mb.master_process_handler=f;
}

#endif

void mb_set_tx_handler(void (*f)(uint8_t *,uint16_t))
{
    mb.tx_handler=f;
}

#if(MB_MODE==MB_MODE_SLAVE)
void mb_error_handler(mb_packet_s* Packet,mb_error_e err)
{
    #ifdef MB_SLAVE_LISTEN_BROADCAST
    if(Packet->unit_id != MB_BROADCAST_ADDRESS)
    #endif
    mb_tx_packet_handler(mb_packet_error(Packet->function,err));
}
#endif

void mb_rx_packet_handler(mb_packet_s Packet)
{
    #if(MB_MODE==MB_MODE_SLAVE)

        mb_error_e err;

        err=mb_check_func(Packet.function);
        if(err){mb_error_handler(&Packet,err);return;}

        #if MB_ENABLE_FUNC_Read_Coils
        if(Packet.function==MB_FUNC_Read_Coils)
        {
            err=mb_check_quantity_2000(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_coils(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Read_Discrete_Inputs
        if(Packet.function==MB_FUNC_Read_Discrete_Inputs)
        {
            err=mb_check_quantity_2000(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_discretes_input_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_discrere_inputs(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Read_Holding_Registers
        if(Packet.function==MB_FUNC_Read_Holding_Registers)
        {
            err=mb_check_quantity_125(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_holding_registers(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Read_Input_Registers
        if(Packet.function==MB_FUNC_Read_Input_Registers)
        {
            err=mb_check_quantity_125(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_input_registers_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_input_registers(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Write_Single_Coil
        if(Packet.function==MB_FUNC_Write_Single_Coil)
        {
            err=mb_check_on_off(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(MB_U16_AT(Packet.payload),0);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_single_coil(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Write_Single_Register
        if(Packet.function==MB_FUNC_Write_Single_Register)
        {
            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload),0);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_single_register(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Write_Multiple_Coils
        if(Packet.function==MB_FUNC_Write_Multiple_Coils)
        {
            err=mb_check_quantity_1968(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_bit_n_byte(MB_U16_AT(Packet.payload+2),Packet.payload[4]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_coils_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_multiple_coils(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        #if MB_ENABLE_FUNC_Write_Multiple_Registers
        if(Packet.function==MB_FUNC_Write_Multiple_Registers)
        {
            err=mb_check_quantity_123(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_reg_n_byte(MB_U16_AT(Packet.payload+2),Packet.payload[4]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_write_multiple_register(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        } else
        #endif

        #if MB_ENABLE_FUNC_Read_Write_Multiple_Registers
        if(Packet.function==MB_FUNC_Read_Write_Multiple_Registers)
        {
            err=mb_check_quantity_125(MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_121(MB_U16_AT(Packet.payload+6));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_quantity_reg_n_byte(MB_U16_AT(Packet.payload+6),Packet.payload[8]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload),MB_U16_AT(Packet.payload+2));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload+4),MB_U16_AT(Packet.payload+6));
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_write_multiple_registers(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}

        } else
        #endif

        #if MB_ENABLE_FUNC_Read_Exception_Status
        if(Packet.function==MB_FUNC_Read_Exception_Status)
        {
            err=mb_slave_process_read_exeption_status(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        } else
        #endif

        #if MB_ENABLE_FUNC_Report_Server_ID
        if(Packet.function==MB_FUNC_Report_Server_ID)
        {
            err=mb_slave_process_report_server_id(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        #endif

        #if MB_ENABLE_FUNC_Encapsulated_Interface
        if(Packet.function==MB_FUNC_Encapsulated_Interface)
        {
            err=mb_check_encapsulated_interface_mei_type(Packet.payload[0]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_ei_device_id_code(Packet.payload[1]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_check_ei_device_o_code(Packet.payload[2]);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_read_device_identification(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        } else
        #endif

        #if MB_ENABLE_FUNC_Mask_Write_Register
        if(Packet.function==MB_FUNC_Mask_Write_Register)
        {
            err=mb_check_table_holding_registers_address(MB_U16_AT(Packet.payload),0);
            if(err){mb_error_handler(&Packet,err);return;}

            err=mb_slave_process_mask_write_register(&Packet);
            if(err){mb_error_handler(&Packet,err);return;}
        }
        else
        #endif

        return;

    #elif(MB_MODE==MB_MODE_MASTER)

        if(mb.master_process_handler!=NULL)
        mb.master_process_handler(Packet);

    #endif
}

