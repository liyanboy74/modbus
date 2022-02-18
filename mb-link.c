/*
 MODBUS Link Layer
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stdio.h>

#include "mb.h"
#include "mb-link.h"
#include "mb-crc.h"
#include "mb-check.h"

uint8_t MB_LINK_Tx_Buffer[MB_LINK_Tx_Buffer_Size];
uint8_t MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Size];

uint8_t MB_LINK_Rx_Buffer_Index=0,MB_LINK_Loop_C=0,MB_LINK_Func=0;
mb_packet_type_e MB_LINK_Packet_Type=MB_PACKET_TYPE_UNKNOWN;

void mb_link_error_handler(mb_link_error_e err)
{
    #ifdef MB_DEBUG
    printf("MB_LINK: ");
    switch(err)
    {
        case MB_LINK_OK:printf("OK!\n");break;
        case MB_LINK_ERROR_Data_Size:printf("Data Size ERROR!\n");break;
        case MB_LINK_ERROR_CRC:printf("CRC Error!\n");break;
        case MB_LINK_ERROR_FUNC:printf("Func not match\n");break;
        case MB_LINK_ERROR_Address:printf("Addres not match!\n");break;
        default:break;
    }
    #endif
    return;
}

void mb_link_send(uint8_t *Data,uint8_t Len)
{
    if(MB_Config.tx_handler!=NULL)
    MB_Config.tx_handler(Data,Len);
}

void mb_link_prepare_tx_data(mb_packet_s Packet)
{
    int i;

    MB_LINK_Tx_Buffer[1]=Packet.func;

    #if(MB_MODE==MB_MODE_SLAVE)

        MB_LINK_Tx_Buffer[0]=mb_slave_address_get();

        if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Var)
        {
            MB_LINK_Tx_Buffer[2]=Packet.len;

            for(i=0;i<Packet.len;i++)
            {
                MB_LINK_Tx_Buffer[i+3]=Packet.Data[i];
            }
            i=mb_crc_add(MB_LINK_Tx_Buffer,i+3);
            mb_link_send(MB_LINK_Tx_Buffer,i);
            return;
        }
        else if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Fix)
        {
            MB_LINK_Tx_Buffer[2]=(Packet.u16_1>>8)&0xff;
            MB_LINK_Tx_Buffer[3]=(Packet.u16_1&0xff);
            MB_LINK_Tx_Buffer[4]=(Packet.u16_2>>8)&0xff;
            MB_LINK_Tx_Buffer[5]=(Packet.u16_2&0xff);
            i=mb_crc_add(MB_LINK_Tx_Buffer,6);
            mb_link_send(MB_LINK_Tx_Buffer,i);
            return;
        }
        else if(Packet.type==MB_PACKET_TYPE_ERROR)
        {
            MB_LINK_Tx_Buffer[2]=Packet.err;
            mb_crc_add(MB_LINK_Tx_Buffer,3);
            mb_link_send(MB_LINK_Tx_Buffer,5);
            return;
        }

    #elif(MB_MODE==MB_MODE_MASTER)

        MB_LINK_Tx_Buffer[0]=Packet.device_address;
        if(Packet.type==MB_PACKET_TYPE_Master_Request_Fix)
        {
            MB_LINK_Tx_Buffer[2]=(Packet.u16_1>>8)&0xff;
            MB_LINK_Tx_Buffer[3]=(Packet.u16_1&0xff);
            MB_LINK_Tx_Buffer[4]=(Packet.u16_2>>8)&0xff;
            MB_LINK_Tx_Buffer[5]=(Packet.u16_2&0xff);
            i=mb_crc_add(MB_LINK_Tx_Buffer,6);
            mb_link_send(MB_LINK_Tx_Buffer,i);
            return;
        }
        else if(Packet.type==MB_PACKET_TYPE_Master_Request_Var)
        {
            MB_LINK_Tx_Buffer[2]=(Packet.u16_1>>8)&0xff;
            MB_LINK_Tx_Buffer[3]=(Packet.u16_1&0xff);
            MB_LINK_Tx_Buffer[4]=(Packet.u16_2>>8)&0xff;
            MB_LINK_Tx_Buffer[5]=(Packet.u16_2&0xff);
            MB_LINK_Tx_Buffer[6]=Packet.len;
            for(i=0;i<Packet.len;i++)
            {
                MB_LINK_Tx_Buffer[i+7]=Packet.Data[i];
            }
            i=mb_crc_add(MB_LINK_Tx_Buffer,i+7);
            mb_link_send(MB_LINK_Tx_Buffer,i);
            return;
        }

    #endif
}

void mb_link_reset_rx_buffer(void)
{
    if(MB_LINK_Rx_Buffer_Index)
    {
        MB_LINK_Rx_Buffer_Index=0;
        MB_LINK_Loop_C=0;
        MB_LINK_Func=0;
        MB_LINK_Packet_Type=MB_PACKET_TYPE_UNKNOWN;
    }
}

void mb_link_check_new_data(uint8_t Byte)
{
    #if(MB_MODE==MB_MODE_MASTER)

        if(!MB_LINK_Rx_Buffer_Index) // Device Address
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;
        }else if(MB_LINK_Rx_Buffer_Index==1) // Func
        {
            MB_LINK_Func=Byte;
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;
            MB_LINK_Packet_Type=mb_get_packet_type(MB_LINK_Func);
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Slave_Responce_Var)
        {
            if(MB_LINK_Rx_Buffer_Index==2) // Size of Data Bytes
            {
                if(Byte>MB_LINK_Rx_MDBL)
                {
                    mb_link_error_handler(MB_LINK_ERROR_Data_Size);
                    mb_link_reset_rx_buffer();
                    return;
                }
                else
                {
                    MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                    MB_LINK_Rx_Buffer_Index++;
                    MB_LINK_Loop_C=Byte+MB_LINK_Rx_Buffer_Index+2;
                }
            }else if((MB_LINK_Loop_C > MB_LINK_Rx_Buffer_Index) && MB_LINK_Loop_C) // Data + CRC
            {
                MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                MB_LINK_Rx_Buffer_Index++;
                if(MB_LINK_Loop_C == MB_LINK_Rx_Buffer_Index) // Data Ready
                {
                    if(mb_crc_check(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index)==MB_CRC_OK)
                    {
                        // OK
                        mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Packet_Type));
                    }
                    else mb_link_error_handler(MB_LINK_ERROR_CRC);
                    mb_link_reset_rx_buffer();
                    return;
                }
            }
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Slave_Responce_Fix)
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;

            if(MB_LINK_Rx_Buffer_Index>=8)
            {
                if(mb_crc_check(MB_LINK_Rx_Buffer,8)==MB_CRC_OK)
                {
                    // OK
                    mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Packet_Type));
                }
                else mb_link_error_handler(MB_LINK_ERROR_CRC);
                mb_link_reset_rx_buffer();
                return;
            }
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_ERROR)
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;

            if(MB_LINK_Rx_Buffer_Index>=5)
            {
                if(mb_crc_check(MB_LINK_Rx_Buffer,5)==MB_CRC_OK)
                {
                    // OK
                    mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Packet_Type));
                }
                else mb_link_error_handler(MB_LINK_ERROR_CRC);
                mb_link_reset_rx_buffer();
                return;
            }
        }
        else // MB Func Not Match!
        {
            mb_link_error_handler(MB_LINK_ERROR_FUNC);
            mb_link_reset_rx_buffer();
            return;
        }

    #elif(MB_MODE==MB_MODE_SLAVE)

        if(!MB_LINK_Rx_Buffer_Index) // Device Address
        {
            if(mb_slave_address_get()==Byte)
            {
                MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                MB_LINK_Rx_Buffer_Index++;
            }
            else mb_link_error_handler(MB_LINK_ERROR_Address);
        }else if(MB_LINK_Rx_Buffer_Index==1) // Func
        {
            MB_LINK_Func=Byte;
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;
            MB_LINK_Packet_Type=mb_get_packet_type(MB_LINK_Func);
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Master_Request_Var)
        {
            if(MB_LINK_Rx_Buffer_Index<6)
            {
                MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                MB_LINK_Rx_Buffer_Index++;
            }
            else if(MB_LINK_Rx_Buffer_Index==6) // Size of Data Bytes
            {
                if(Byte>MB_LINK_Rx_MDBL)
                {
                    mb_link_error_handler(MB_LINK_ERROR_Data_Size);
                    mb_link_reset_rx_buffer();
                    return;
                }
                else
                {
                    MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                    MB_LINK_Rx_Buffer_Index++;
                    MB_LINK_Loop_C=Byte+MB_LINK_Rx_Buffer_Index+2;
                }
            }else if((MB_LINK_Loop_C > MB_LINK_Rx_Buffer_Index) && MB_LINK_Loop_C) // Data + CRC
            {
                MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
                MB_LINK_Rx_Buffer_Index++;
                if(MB_LINK_Loop_C == MB_LINK_Rx_Buffer_Index) // Data Ready
                {
                    if(mb_crc_check(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index)==MB_CRC_OK)
                    {
                        // OK
                        mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Packet_Type));
                    }
                    else mb_link_error_handler(MB_LINK_ERROR_CRC);
                    mb_link_reset_rx_buffer();
                    return;
                }
            }
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Master_Request_Fix)
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;

            if(MB_LINK_Rx_Buffer_Index>=8)
            {
                if(mb_crc_check(MB_LINK_Rx_Buffer,8)==MB_CRC_OK)
                {
                    // OK
                    mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Packet_Type));
                }
                else mb_link_error_handler(MB_LINK_ERROR_CRC);
                mb_link_reset_rx_buffer();
                return;
            }
        }
        else // MB Func Not Match!
        {
            mb_link_error_handler(MB_LINK_ERROR_FUNC);
            mb_link_reset_rx_buffer();
            return;
        }
    #endif
}

mb_packet_s mb_rx_packet_split(uint8_t *Packet_Buffer,mb_packet_type_e Packet_Type)
{
    mb_packet_s Packet;

    Packet.device_address=Packet_Buffer[0];
    Packet.func=Packet_Buffer[1];
    Packet.type=Packet_Type;

    if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Var)
    {
        Packet.len=Packet_Buffer[2];
        Packet.Data=&Packet_Buffer[3];
    }
    else if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Fix||Packet.type==MB_PACKET_TYPE_Master_Request_Fix)
    {
        Packet.u16_1=(Packet_Buffer[2]<<8)|(Packet_Buffer[3]);
        Packet.u16_2=(Packet_Buffer[4]<<8)|(Packet_Buffer[5]);
    }
    else if(Packet.type==MB_PACKET_TYPE_Master_Request_Var)
    {
        Packet.u16_1=(Packet_Buffer[2]<<8)|(Packet_Buffer[3]);
        Packet.u16_2=(Packet_Buffer[4]<<8)|(Packet_Buffer[5]);
        Packet.len=Packet_Buffer[6];
        Packet.Data=&Packet_Buffer[7];
    }
    else if(Packet.type==MB_PACKET_TYPE_ERROR)
    {
        Packet.err=Packet_Buffer[2];
    }

    return Packet;
}

mb_packet_type_e mb_get_packet_type(mb_functions_e Func)
{
    #if(MB_MODE==MB_MODE_MASTER)

        if(Func==MB_FUNC_Read_Coils||Func==MB_FUNC_Read_Discrete_Inputs||Func==MB_FUNC_Read_Holding_Registers||Func==MB_FUNC_Read_Input_Registers)
        {
            return MB_PACKET_TYPE_Slave_Responce_Var;
        }
        else if(Func==MB_FUNC_Write_Single_Coil||Func==MB_FUNC_Write_Single_Register||Func==MB_FUNC_Write_Multiple_Coils||Func==MB_FUNC_Write_Multiple_Registers)
        {
            return MB_PACKET_TYPE_Slave_Responce_Fix;
        }
        else if(Func&0x80)
        {
            return MB_PACKET_TYPE_ERROR;
        }

    #elif(MB_MODE==MB_MODE_SLAVE)

        if(Func==MB_FUNC_Write_Multiple_Coils||Func==MB_FUNC_Write_Multiple_Registers)
        {
            return MB_PACKET_TYPE_Master_Request_Var;
        }
        else if(Func==MB_FUNC_Read_Coils||Func==MB_FUNC_Read_Discrete_Inputs||Func==MB_FUNC_Read_Holding_Registers||Func==MB_FUNC_Read_Input_Registers||Func==MB_FUNC_Write_Single_Coil||Func==MB_FUNC_Write_Single_Register)
        {
            return MB_PACKET_TYPE_Master_Request_Fix;
        }

    #endif
    return MB_PACKET_TYPE_UNKNOWN;
}

