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
    switch(err)
    {
        case MB_LINK_OK:printf("OK!\n");break;
        case MB_LINK_ERROR_Data_Size:printf("Data Size ERROR!\n");break;
        case MB_LINK_ERROR_CRC:printf("CRC Error!");break;
        case MB_LINK_ERROR_FUNC:printf("Func not match\n");break;
        case MB_LINK_ERROR_Address:printf("Addres not match!\n");break;
        default:break;
    }
    #endif
    return;
}

mb_error_e mb_link_send(uint8_t *Data,uint8_t Len)
{
    #ifdef MB_DEBUG
    int i;
    printf("TX: ");
    for(i=0;i<Len;i++)printf("%02x ",Data[i]);
    printf("\n");
    #endif
    return MB_OK;
}

mb_error_e mb_link_prepare_tx_data(mb_packet_s Packet)
{
    int i;

    MB_LINK_Tx_Buffer[1]=Packet.func;

    if(mb_mode_get()==MB_MODE_SLAVE)
    {
        MB_LINK_Tx_Buffer[0]=mb_slave_address_get();

        if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Var)
        {
            MB_LINK_Tx_Buffer[2]=Packet.len;

            for(i=0;i<Packet.len;i++)
            {
                MB_LINK_Tx_Buffer[i+3]=Packet.Data[i];
            }
            i=mb_crc_add(MB_LINK_Tx_Buffer,i+3);
            return mb_link_send(MB_LINK_Tx_Buffer,i);
        }
        else if(Packet.type==MB_PACKET_TYPE_Slave_Responce_Fix)
        {
            MB_LINK_Tx_Buffer[2]=(Packet.u16_1>>8)&0xff;
            MB_LINK_Tx_Buffer[3]=(Packet.u16_1&0xff);
            MB_LINK_Tx_Buffer[4]=(Packet.u16_2>>8)&0xff;
            MB_LINK_Tx_Buffer[5]=(Packet.u16_2&0xff);
            i=mb_crc_add(MB_LINK_Tx_Buffer,6);
            return mb_link_send(MB_LINK_Tx_Buffer,i);
        }
    }
    return MB_ERROR_SLAVE_DEVICE_FAILURE;
}

void mb_link_reset_rx_buffer(void)
{
    MB_LINK_Rx_Buffer_Index=0;
    MB_LINK_Loop_C=0;
    MB_LINK_Func=0;
    MB_LINK_Packet_Type=MB_PACKET_TYPE_UNKNOWN;
}

void mb_link_check_new_data(uint8_t Byte)
{
    if(mb_mode_get()==MB_MODE_MASTER)
    {
        if(!MB_LINK_Rx_Buffer_Index) // Device Address
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;
        }else if(MB_LINK_Rx_Buffer_Index==1) // Func
        {
            MB_LINK_Func=Byte;
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;
            MB_LINK_Packet_Type=mb_get_packet_type(MB_MODE_MASTER,MB_LINK_Func);
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Master_Responce_Var)
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
                        mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index-2,MB_LINK_Packet_Type));
                    }
                    else mb_link_error_handler(MB_LINK_ERROR_CRC);
                    mb_link_reset_rx_buffer();
                    return;
                }
            }
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Master_Responce_Fix)
        {
            MB_LINK_Rx_Buffer[MB_LINK_Rx_Buffer_Index]=Byte;
            MB_LINK_Rx_Buffer_Index++;

            if(MB_LINK_Rx_Buffer_Index>=8)
            {
                if(mb_crc_check(MB_LINK_Rx_Buffer,8)==MB_CRC_OK)
                {
                    // OK
                    mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index-2,MB_LINK_Packet_Type));
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
    }
    else // SLAVE Mode
    {
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
            MB_LINK_Packet_Type=mb_get_packet_type(MB_MODE_SLAVE,MB_LINK_Func);
        }
        else if(MB_LINK_Packet_Type==MB_PACKET_TYPE_Slave_Responce_Var)
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
                        mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index-2,MB_LINK_Packet_Type));
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
                    mb_rx_packet_handler(mb_rx_packet_split(MB_LINK_Rx_Buffer,MB_LINK_Rx_Buffer_Index-2,MB_LINK_Packet_Type));
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
    }
}



