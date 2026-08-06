/*
 MODBUS RTU Layer
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#include <stdio.h>
#include "mb-rtu.h"
#include "mb-crc.h"

uint8_t MB_RTU_Tx_Buffer[MB_RTU_Tx_Buffer_Size];
uint8_t MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Size];

uint8_t MB_RTU_Rx_Buffer_Index=0,MB_RTU_Loop_C=0,MB_RTU_Func=0;
mb_rtu_error_e MB_RTU_Status=MB_RTU_OK;

static void mb_rtu_get_rx_info(mb_rtu_info_s *v,uint8_t f)
{
    switch ((mb_function_e)f)
    {
        #if(MB_MODE==MB_MODE_MASTER)
        case MB_FUNC_Read_Coils:
        case MB_FUNC_Read_Discrete_Inputs:
        case MB_FUNC_Read_Holding_Registers:
        case MB_FUNC_Read_Input_Registers:
            v->index=2;
            v->type=MB_RTU_TYPE_VAR;
            break;
        case MB_FUNC_Write_Single_Coil:
        case MB_FUNC_Write_Single_Register:
        case MB_FUNC_Write_Multiple_Coils:
        case MB_FUNC_Write_Multiple_Registers:
            v->index=8;
            v->type=MB_RTU_TYPE_FIX;
            break;
        #elif(MB_MODE==MB_MODE_SLAVE)
        case MB_FUNC_Write_Multiple_Coils:
        case MB_FUNC_Write_Multiple_Registers:
            v->index=6;
            v->type=MB_RTU_TYPE_VAR;
            break;
        case MB_FUNC_Read_Write_Multiple_Registers:
            v->index=10;
            v->type=MB_RTU_TYPE_VAR;
            break;
        case MB_FUNC_Read_Coils:
        case MB_FUNC_Read_Discrete_Inputs:
        case MB_FUNC_Read_Holding_Registers:
        case MB_FUNC_Read_Input_Registers:
        case MB_FUNC_Write_Single_Coil:
        case MB_FUNC_Write_Single_Register:
            v->index=8;
            v->type=MB_RTU_TYPE_FIX;
            break;
        case MB_FUNC_Encapsulated_Interface:
            v->index=7;
            v->type=MB_RTU_TYPE_FIX;
            break;
        case MB_FUNC_Mask_Write_Register:
            v->index=10;
            v->type=MB_RTU_TYPE_FIX;
            break;
        case MB_FUNC_Read_Exception_Status:
        case MB_FUNC_Report_Server_ID:
            v->index=4;
            v->type=MB_RTU_TYPE_FIX;
            break;
        #endif
        default:
            v->index=0;
            v->type=MB_RTU_TYPE_NONE;
        break;
    }
    #if(MB_MODE==MB_MODE_MASTER)
    if(MB_RTU_Func & 0x80)
    {
        v->type=MB_RTU_TYPE_FIX;
        v->index=5;
    }
    #endif
}

static void mb_rtu_error_handler(mb_rtu_error_e err)
{
    MB_RTU_Status=err;
    #ifdef MB_RTU_DEBUG
    printf("MB_LINK: ");
    switch(err)
    {
        case MB_RTU_OK:printf("OK!\n");break;
        case MB_RTU_ERROR_Data_Size:printf("Data Size ERROR!\n");break;
        case MB_RTU_ERROR_CRC:printf("CRC Error!\n");break;
        case MB_RTU_ERROR_FUNC:printf("Func not match\n");break;
        case MB_RTU_ERROR_Address:printf("Addres not match!\n");break;
        default:break;
    }
    #endif
    return;
}

void mb_rtu_send(uint8_t *Data,uint8_t Len)
{
    if(mb.tx_handler!=NULL)
    mb.tx_handler(Data,Len);
}

static void mb_rtu_prepare_tx_data(mb_packet_s Packet)
{
    uint8_t i;

    MB_RTU_Tx_Buffer[1]=Packet.function;
    #if (MB_MODE==MB_MODE_SLAVE)
        MB_RTU_Tx_Buffer[0]=mb.address;
    #endif
    #if (MB_MODE==MB_MODE_MASTER)
        MB_RTU_Tx_Buffer[0]=Packet.unit_id;
    #endif
    for(i=0;i<Packet.length;i++)
    {
        MB_RTU_Tx_Buffer[i+2]=Packet.payload[i];
    }
    mb_crc_add(MB_RTU_Tx_Buffer,i+2);   //+2=ID+FUNC
    mb_rtu_send(MB_RTU_Tx_Buffer,i+4);  //+2=CRC
    return;
}

void mb_rtu_reset_rx_buffer(void)
{
    if(MB_RTU_Rx_Buffer_Index)
    {
        MB_RTU_Rx_Buffer_Index=0;
        MB_RTU_Loop_C=0;
        MB_RTU_Func=0;
    }
}

static mb_packet_s mb_rtu_rx_packet_split(uint8_t *Packet_Buffer,uint8_t Len)
{
    mb_packet_s Packet;

    Packet.unit_id=Packet_Buffer[0];
    Packet.function=(mb_function_e)Packet_Buffer[1];
    Packet.length=Len-2;//remove unit_id & function
    Packet.payload=&Packet_Buffer[2];
    return Packet;
}

void mb_rtu_check_new_data(uint8_t oneByte)
{
    static mb_rtu_info_s info;

    if(!MB_RTU_Rx_Buffer_Index) //Device Address
    {
        #if(MB_MODE==MB_MODE_MASTER)
        MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
        MB_RTU_Rx_Buffer_Index++;
        #elif(MB_MODE==MB_MODE_SLAVE)
        if(mb.address==oneByte
        #ifdef MB_SLAVE_LISTEN_BROADCAST
        || oneByte == MB_BROADCAST_ADDRESS
        #endif
        )
        {
            MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
            MB_RTU_Rx_Buffer_Index++;
        }
        else mb_rtu_error_handler(MB_RTU_ERROR_Address);
        #endif
    }
    else if(MB_RTU_Rx_Buffer_Index==1) //Func
    {
        MB_RTU_Func=oneByte;
        MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
        MB_RTU_Rx_Buffer_Index++;
        mb_rtu_get_rx_info(&info,MB_RTU_Func);
    }
    else 
    {
        if(info.type==MB_RTU_TYPE_VAR)
        {
            if(MB_RTU_Rx_Buffer_Index<info.index)
            {
                MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
                MB_RTU_Rx_Buffer_Index++;
            }
            else if(MB_RTU_Rx_Buffer_Index==info.index)
            {
                if(oneByte>MB_RTU_Rx_MDBL)
                {
                    mb_rtu_error_handler(MB_RTU_ERROR_Data_Size);
                    mb_rtu_reset_rx_buffer();
                }
                else
                {
                    MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
                    MB_RTU_Rx_Buffer_Index++;
                    MB_RTU_Loop_C=(uint8_t)(oneByte+MB_RTU_Rx_Buffer_Index+2);
                }
            }
            else if((MB_RTU_Loop_C > MB_RTU_Rx_Buffer_Index) && MB_RTU_Loop_C) //Data + CRC
            {
                MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
                MB_RTU_Rx_Buffer_Index++;
                if(MB_RTU_Loop_C == MB_RTU_Rx_Buffer_Index) //Data Ready
                {
                    if(mb_crc_check(MB_RTU_Rx_Buffer,MB_RTU_Rx_Buffer_Index)==MB_CRC_OK)
                    {
                        //OK -> Remove CRC & Go!
                        mb_rx_packet_handler(mb_rtu_rx_packet_split(MB_RTU_Rx_Buffer,MB_RTU_Rx_Buffer_Index-2));
                    }
                    else mb_rtu_error_handler(MB_RTU_ERROR_CRC);
                    mb_rtu_reset_rx_buffer();
                }
            }
        }
        else if(info.type==MB_RTU_TYPE_FIX)
        {
            MB_RTU_Rx_Buffer[MB_RTU_Rx_Buffer_Index]=oneByte;
            MB_RTU_Rx_Buffer_Index++;

            if(MB_RTU_Rx_Buffer_Index>=info.index)
            {
                if(mb_crc_check(MB_RTU_Rx_Buffer,MB_RTU_Rx_Buffer_Index)==MB_CRC_OK)
                {
                    //OK -> Remove CRC & Go!
                    mb_rx_packet_handler(mb_rtu_rx_packet_split(MB_RTU_Rx_Buffer,MB_RTU_Rx_Buffer_Index-2));
                }
                else mb_rtu_error_handler(MB_RTU_ERROR_CRC);
                mb_rtu_reset_rx_buffer();
            }
        }
        else
        {
            //MB Func Not Match!
            mb_rtu_error_handler(MB_RTU_ERROR_FUNC);
            mb_rtu_reset_rx_buffer();
        }
    }
}

//RTU Transport
void mb_tx_packet_handler(mb_packet_s Packet)
{
    mb_rtu_prepare_tx_data(Packet);
}
