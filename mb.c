#include "mb.h"
#include "table.h"
#include "crc16.h"

mb_error_e mb_check_func(uint8_t func)
{
    switch (func)
    {
        case MB_Read_Coils:break;
        case MB_Read_Discrete_Inputs:break;
        case MB_Read_Holding_Registers:break;
        case MB_Read_Input_Registers:break;
        case MB_Write_Single_Coil:break;
        case MB_Write_Single_Register:break;
        case MB_Write_Multiple_Coils:break;
        case MB_Write_Multiple_Registers:break;
        default:return MB_ERROR_ILLEGAL_FUNCTION;
    }
    return MB_ERROR_OK;
}

mb_error_e mb_check_quantity(uint16_t Quantity)
{
    if(Quantity>=0x0001&&Quantity<=0x07d0)return MB_ERROR_OK;
    return MB_ERROR_ILLEGAL_DATA_VALUE;
}

mb_error_e mb_check_table_bit_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>=(TBALE_Coils_Size*TABLE_Sel_BitSize))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_ERROR_OK;
}

mb_error_e mb_check_table_address(uint16_t StartAd,uint16_t Quantity)
{
    if(StartAd>=(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    if((StartAd+Quantity)>=(TBALE_Coils_Size))return MB_ERROR_ILLEGAL_DATA_ADDRESS;
    return MB_ERROR_OK;
}

mb_packet_len mb_add_crc(uint8_t *Buffer,mb_packet_len Len)
{
    uint16_t crc;

    crc=mb_crc16(Buffer,Len);

    Buffer[Len]= crc&0xff;
	Buffer[Len+1]=(crc>>8)&0xff;

    return Len+2;
}

mb_crc_e mb_check_crc(uint8_t *Buffer,mb_packet_len Len)
{
    uint16_t *Pcrc,Tcrc;
    Tcrc=mb_crc16(Buffer,Len-2);
    Pcrc=(uint16_t*)(&Buffer[Len-2]);
    if(Tcrc==*Pcrc)return MB_CRC_OK;
    return MB_CRC_ERROR;
}

mb_process_s* mb_process(mb_process_s* Obj)
{
    Obj->err=mb_check_func(Obj->func);
    if(Obj->err)return Obj;

    return Obj;
}
