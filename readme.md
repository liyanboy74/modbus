# Mod Bus

Includes:

```c
#include "mb.h"
```



Set Callback:
```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	mb_uart_rxCpltCallback(huart);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	mb_uart_txCpltCallback(huart);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2)
	{
		mb_tim_periodElapsedCallback(htim);
	}
}
```



Usage Example:

```c
//Global
uint16_t MB_ANALOG_INPUT_REGISTERS[8];
.
.
//main()
int i,j,k;
uint8_t data[32];
uint16_t Addres,Len,value;
.
.
//init
mb_init(MB_MODE_RTU,&huart3,&htim2);
.
.
//in loop
if(MB_UART_STATE==MB_UART_READY)
{
    for(i=0;i<mb_get_num_pakets();i++)
    {
        if(mb_check_crc16(i))
        {
            switch (mb_rx_buffer[i][MB_BIT_FUNCTION])
            {
                case MB_Read_Coils :
                    Addres=(mb_rx_buffer[i][2]<<8)|(mb_rx_buffer[i][3]);
                    Len=(mb_rx_buffer[i][4]<<8)|(mb_rx_buffer[i][5]);
                    if(Addres==0)
                    {
                        if(Len<=8)
                        {
                            memset(data,0,1);
                            data[0]=HAL_GPIO_ReadPin(LED_GPIO_Port,LED_Pin)&0x01;
                            mb_rtu_response_read_coil(&huart3,MB_SLAVE_ID,1,data);
                        }
                    }
                    break;

                case MB_Write_Single_Coil:
                    Addres=(mb_rx_buffer[i][2]<<8)|(mb_rx_buffer[i][3]);
                    value=(mb_rx_buffer[i][4]<<8)|(mb_rx_buffer[i][5]);
                    if(Addres==0)
                    {
                        if(!value)
                        {
                            HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
                        }
                        else
                        {
                            HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
                        }
                        mb_rtu_response_write_single_coil(&huart3,MB_SLAVE_ID,Addres,value);
                    }
                    break;

                case MB_Read_Input_Registers:
                    Addres=(mb_rx_buffer[i][2]<<8)|(mb_rx_buffer[i][3]);
                    Len=(mb_rx_buffer[i][4]<<8)|(mb_rx_buffer[i][5]);

                    for(k=0,j=Addres;(j<(Addres+Len))&&(Addres+Len<=8);j++)
                    {
                        data[k]=(MB_ANALOG_INPUT_REGISTERS[j]>>8)&0xff;
                        k++;
                        data[k]=(MB_ANALOG_INPUT_REGISTERS[j]>>0)&0xff;
                        k++;
                    }

                    if(k!=0)mb_rtu_response_read_input_registers(&huart3,MB_SLAVE_ID,k,data);

                    break;

                default:mb_log("FUNCTION not found!\r\n");
            }		
        }		
        else
        {
            mb_log("CRC Check Error!\r\n");
            uint16_t crc=CRC16(mb_rx_buffer[i],mb_get_size_pakets(i)-2);;
            sprintf((char*)data,(char *)"CRC=%02X%02X\r\n",(char)(crc&0xff),(char)(crc>>8));
            mb_log((char *)data);

        }
    }		
    mb_uart_buffer_reset();
}
else if(MB_UART_STATE==MB_UART_BUFF_IS_FULL)
{
    mb_log("UBO!\r\n");
    mb_uart_buffer_reset();
}	
```



