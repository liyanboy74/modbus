#include "mb-check.h"
#include "mb-table.h"

mb_error_e mb_check_func(uint8_t func)
{
    switch (func)
    {
        case MB_FUNC_Read_Coils:break;
        case MB_FUNC_Read_Discrete_Inputs:break;
        case MB_FUNC_Read_Holding_Registers:break;
        case MB_FUNC_Read_Input_Registers:break;
        case MB_FUNC_Write_Single_Coil:break;
        case MB_FUNC_Write_Single_Register:break;
        case MB_FUNC_Write_Multiple_Coils:break;
        case MB_FUNC_Write_Multiple_Registers:break;
        default:return MB_ERROR_ILLEGAL_FUNCTION;
    }
    return MB_OK;
}

mb_error_e mb_check_quantity(uint16_t Quantity)
{
    if(Quantity>=0x0001&&Quantity<=0x07d0)return MB_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_quantity_bit_n_byte(uint16_t Quantity,uint8_t N)
{
    uint8_t n;
    n=Quantity/8;
    if(Quantity%8)n++;
    if(n!=N)return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}

mb_error_e mb_check_quantity_reg_n_byte(uint16_t Quantity,uint8_t N)
{
    if((Quantity*2)!=N)return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}

mb_error_e mb_check_table_bit_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_table_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>=(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_OK;
}

mb_error_e mb_check_on_off(uint16_t Value)
{
    if(Value != 0xff00 && Value!=0x000) return MB_ERROR_ILLEGAL_DATA_VALUE;
    return MB_OK;
}
