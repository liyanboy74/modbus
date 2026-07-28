/*
 MODBUS Check Data and Addres
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include "mb-check.h"
#include "mb-table.h"

#if(MB_MODE==MB_MODE_SLAVE)

mb_error_e mb_check_func(uint8_t func)
{
    switch (func)
    {
        #if MB_ENABLE_FUNC_Read_Coils
        case MB_FUNC_Read_Coils:break;
        #endif

        #if MB_ENABLE_FUNC_Read_Discrete_Inputs
        case MB_FUNC_Read_Discrete_Inputs:break;
        #endif

        #if MB_ENABLE_FUNC_Read_Holding_Registers
        case MB_FUNC_Read_Holding_Registers:break;
        #endif

        #if MB_ENABLE_FUNC_Read_Input_Registers
        case MB_FUNC_Read_Input_Registers:break;
        #endif

        #if MB_ENABLE_FUNC_Write_Single_Coil
        case MB_FUNC_Write_Single_Coil:break;
        #endif

        #if MB_ENABLE_FUNC_Write_Single_Register
        case MB_FUNC_Write_Single_Register:break;
        #endif

        #if MB_ENABLE_FUNC_Write_Multiple_Coils
        case MB_FUNC_Write_Multiple_Coils:break;
        #endif

        #if MB_ENABLE_FUNC_Write_Multiple_Registers
        case MB_FUNC_Write_Multiple_Registers:break;
        #endif

        #if MB_ENABLE_FUNC_Read_Write_Multiple_Registers
        case MB_FUNC_Read_Write_Multiple_Registers:break;
        #endif

        #if MB_ENABLE_FUNC_Read_Exception_Status
        case MB_FUNC_Read_Exception_Status:break;
        #endif

        #if MB_ENABLE_FUNC_Encapsulated_Interface
        case MB_FUNC_Encapsulated_Interface:break;
        #endif

        #if MB_ENABLE_FUNC_Report_Server_ID
        case MB_FUNC_Report_Server_ID:break;
        #endif
        
        default:return MB_ERROR_ILLEGAL_FUNCTION;
    }
    return MB_OK;
}

mb_error_e mb_check_quantity_121(uint16_t Quantity)
{
	//1 to 121  (0X0079), for  (0x17) Read/Write Multiple registers
    if(Quantity>=MB_MIN_QUANTITY&&Quantity<=MB_MAX_QUANTITY_121)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_123(uint16_t Quantity)
{
	//1 to 123  (0x007B), for (0x10) Write Multiple registers
    if(Quantity>=MB_MIN_QUANTITY&&Quantity<=MB_MAX_QUANTITY_123)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_125(uint16_t Quantity)
{
	//1 to 125  (0x7D), for (0x03) Read Holding Registers, (0x04) Read Input Registers
    if(Quantity>=MB_MIN_QUANTITY&&Quantity<=MB_MAX_QUANTITY_125)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_1968(uint16_t Quantity)
{
	//1 to 1968 (0x07B0), for (0x0F) Write Multiple Coils
    if(Quantity>=MB_MIN_QUANTITY&&Quantity<=MB_MAX_QUANTITY_1968)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_2000(uint16_t Quantity)
{
	//1 to 2000 (0x7D0), for (0x01) Read Coils ,(0x02) Read Discrete Inputs
    if(Quantity>=MB_MIN_QUANTITY&&Quantity<=MB_MAX_QUANTITY_2000)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_bit_n_byte(uint16_t Quantity,uint8_t N)
{
    uint8_t n;
    n=(uint8_t)(Quantity/8);
    if(Quantity%8)n++;
    if(n!=N)return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}

mb_error_e mb_check_quantity_reg_n_byte(uint16_t Quantity,uint8_t N)
{
    if((Quantity*2)!=N)return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}

mb_error_e mb_check_table_coils_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_table_discretes_input_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Discretes_Input_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>(TBALE_Discretes_Input_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_table_input_registers_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Input_Registers_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>(TBALE_Input_Registers_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_table_holding_registers_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TABLE_Holding_Registers_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>(TABLE_Holding_Registers_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_on_off(uint16_t Value)
{
    if(Value != MB_COIL_ON && Value != MB_COIL_OFF) return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}

mb_error_e mb_check_encapsulated_interface_mei_type(uint8_t mei)
{
    if(mei==0x0e) return MB_OK; // Read Device Identification
    else return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_ei_device_id_code(uint8_t id)
{
    if(id==0x01) return MB_OK; // Basic Device Identification
    else return MB_ERROR_ILLEGAL_DATA_ADDRESS;
}

mb_error_e mb_check_ei_device_o_code(uint8_t o_id)
{
    if(o_id<3) return MB_OK; // Max object code
    else return MB_ERROR_ILLEGAL_DATA_ADDRESS;
}

#endif
