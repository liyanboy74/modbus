#include <stdio.h>

#include "mb.h"
#include "mb-link.h"
#include "mb-crc.h"
#include "mb-check.h"

#include "fifo.h"


uint8_t MB_LINK_Buffer[MB_LINK_Buffer_Size];
uint8_t MB_LINK_BufferIndex=0,MB_LINK_Loop_C=0,MB_LINK_Func=0;

void mb_link_error_handler(mb_link_error_e err)
{
    #ifdef MB_DEBUG
    switch(err)
    {
        case MB_LINK_OK:printf("OK!\n");break;
        case MB_LINK_ERROR_Data_Size:printf("Data Size ERROR!\n");break;
        case MB_LINK_ERROR_CRC:printf("CRC Error!");break;
        case MB_LINK_ERROR_FUNC:printf("Func not match\n");break;
        case MB_LINK_ERROR_Address:printf("Addres not match!\n");break;
    }
    #endif
    return;
}

void mb_link_reset_buffer(void)
{
    MB_LINK_BufferIndex=0;
    MB_LINK_Loop_C=0;
    MB_LINK_Func=0;
}

void mb_link_check_new_data(uint8_t Byte)
{
    if(MB_Config.mode==MB_MODE_MASTER)
    {
        if(!MB_LINK_BufferIndex) // Device Address
        {
            MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
            MB_LINK_BufferIndex++;
        }else if(MB_LINK_BufferIndex==1) // Func
        {
            MB_LINK_Func=Byte;
            MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
            MB_LINK_BufferIndex++;
        }
        else if(MB_LINK_Func==MB_Read_Coils||MB_LINK_Func==MB_Read_Discrete_Inputs||MB_LINK_Func==MB_Read_Holding_Registers||MB_LINK_Func==MB_Read_Input_Registers)
        {
            if(MB_LINK_BufferIndex==2) // Size of Data Bytes
            {
                if(Byte>MB_LINK_Max_Data_Bytes_Len)
                {
                    mb_link_error_handler(MB_LINK_ERROR_Data_Size);
                    mb_link_reset_buffer();
                }
                else
                {
                    MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                    MB_LINK_BufferIndex++;
                    MB_LINK_Loop_C=Byte+MB_LINK_BufferIndex+2;
                }
            }else if((MB_LINK_Loop_C > MB_LINK_BufferIndex) && MB_LINK_Loop_C) // Data + CRC
            {
                MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                MB_LINK_BufferIndex++;
                if(MB_LINK_Loop_C == MB_LINK_BufferIndex) // Data Ready
                {
                    if(mb_check_crc(MB_LINK_Buffer,MB_LINK_BufferIndex)==MB_CRC_OK)
                    {
                        // OK
                        mb_link_error_handler(MB_LINK_OK);
                    }
                    else mb_link_error_handler(MB_LINK_ERROR_CRC);
                    mb_link_reset_buffer();
                }
            }
        }
        else if(MB_LINK_Func==MB_Write_Single_Coil||MB_LINK_Func==MB_Write_Single_Register||MB_LINK_Func==MB_Write_Multiple_Coils||MB_LINK_Func==MB_Write_Multiple_Registers)
        {
            MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
            MB_LINK_BufferIndex++;

            if(MB_LINK_BufferIndex>=8)
            {
                if(mb_check_crc(MB_LINK_Buffer,8)==MB_CRC_OK)
                {
                    // OK
                    mb_link_error_handler(MB_LINK_OK);
                }
                else mb_link_error_handler(MB_LINK_ERROR_CRC);
                mb_link_reset_buffer();
            }
        }
        else // MB Func Not Match!
        {
            mb_link_error_handler(MB_LINK_ERROR_FUNC);
            mb_link_reset_buffer();
        }
    }
    else // SLAVE Mode
    {
        if(!MB_LINK_BufferIndex) // Device Address
        {
            if(MB_Config.Address==Byte)
            {
                MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                MB_LINK_BufferIndex++;
            }
            else mb_link_error_handler(MB_LINK_ERROR_Address);
        }else if(MB_LINK_BufferIndex==1) // Func
        {
            MB_LINK_Func=Byte;
            MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
            MB_LINK_BufferIndex++;
        }
        else if(MB_LINK_Func==MB_Write_Multiple_Coils||MB_LINK_Func==MB_Write_Multiple_Registers)
        {
            if(MB_LINK_BufferIndex<6)
            {
                MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                MB_LINK_BufferIndex++;
            }
            else if(MB_LINK_BufferIndex==6) // Size of Data Bytes
            {
                if(Byte>MB_LINK_Max_Data_Bytes_Len)
                {
                    mb_link_error_handler(MB_LINK_ERROR_Data_Size);
                    mb_link_reset_buffer();
                }
                else
                {
                    MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                    MB_LINK_BufferIndex++;
                    MB_LINK_Loop_C=Byte+MB_LINK_BufferIndex+2;
                }
            }else if((MB_LINK_Loop_C > MB_LINK_BufferIndex) && MB_LINK_Loop_C) // Data + CRC
            {
                MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
                MB_LINK_BufferIndex++;
                if(MB_LINK_Loop_C == MB_LINK_BufferIndex) // Data Ready
                {
                    if(mb_check_crc(MB_LINK_Buffer,MB_LINK_BufferIndex)==MB_CRC_OK)
                    {
                        // OK
                        mb_link_error_handler(MB_LINK_OK);
                    }
                    else mb_link_error_handler(MB_LINK_ERROR_CRC);
                    mb_link_reset_buffer();
                }
            }
        }
        else if(MB_LINK_Func==MB_Read_Coils||MB_LINK_Func==MB_Read_Discrete_Inputs||MB_LINK_Func==MB_Read_Holding_Registers||MB_LINK_Func==MB_Read_Input_Registers||MB_LINK_Func==MB_Write_Single_Coil||MB_LINK_Func==MB_Write_Single_Register)
        {
            MB_LINK_Buffer[MB_LINK_BufferIndex]=Byte;
            MB_LINK_BufferIndex++;

            if(MB_LINK_BufferIndex>=8)
            {
                if(mb_check_crc(MB_LINK_Buffer,8)==MB_CRC_OK)
                {
                    // OK
                    mb_link_error_handler(MB_LINK_OK);
                }
                else mb_link_error_handler(MB_LINK_ERROR_CRC);
                mb_link_reset_buffer();
            }
        }
        else // MB Func Not Match!
        {
            mb_link_error_handler(MB_LINK_ERROR_FUNC);
            mb_link_reset_buffer();
        }
    }
}



