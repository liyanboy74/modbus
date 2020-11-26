#include "mb.h"
#include "string.h"
//HAL_Status
HAL_StatusTypeDef HStatus;
//--------------------   mb buffer   ---------------------------------
uint8_t mb_tx_buffer[mb_tx_buffer_size];
uint8_t mb_rx_buffer_size[mb_rx_buffer_size_y];
uint8_t mb_rx_buffer[mb_rx_buffer_size_y][mb_rx_buffer_size_x];
uint8_t mb_tx_buffer_conter=0;
uint8_t mb_rx_buffer_conter_x=0;
uint8_t mb_rx_buffer_conter_y=0;
//--------------------  Mod Bus Param  ------------------------------
uint8_t MB_SLAVE_ID=MB_Def_Slave_ID;
uint8_t MB_UART_BUFF_CH;
uint8_t MB_FUNCTION,MB_LEN;
MB_UART_STATUS_typedef MB_UART_STATE;
//------------ mb_uart_reset_char_listen (privit) --------------------	
void mb_uart_reset_char_listen(UART_HandleTypeDef *huart)
{
	do{
	HStatus=HAL_UART_Receive_IT(huart,&MB_UART_BUFF_CH,1);
	}while(HStatus!=HAL_OK);
	mb_hal_tim.Instance->CNT=0x0000;
}
//--------------------- TIM INTRUPT ----------------------------------
void mb_tim_periodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(MB_UART_STATE!=MB_UART_LISTEN&&MB_UART_STATE!=MB_UART_READY&&
		MB_UART_STATE!=MB_UART_BUFF_IS_FULL)
	{
		if(mb_rx_buffer_conter_y>=1&&mb_rx_buffer_size[mb_rx_buffer_conter_y]==0)
		{
			mb_rx_buffer_conter_y--;
			MB_UART_STATE=MB_UART_READY;
		}
		else MB_UART_STATE=MB_UART_LISTEN;
	}
	else
	{
		//do somting
	}
}
//---------------------- mb_get_num_pakets ----------------------------
uint8_t mb_get_num_pakets()
{
	uint8_t i=0;
	
	while(i<=mb_rx_buffer_conter_y)
	{
		if(mb_rx_buffer_size[i]==0)break;
		else i++;
	}
return i;	
}
//----------------------------------------------------------------------
uint8_t mb_get_size_pakets(uint8_t address)
{
	return mb_rx_buffer_size[address];
}
//--------------- mb_check_start (privit) ------------------------------
uint8_t mb_check_start()
{
	if(mb_hal_tim.Instance->CNT >
		((float)10000.0/(mb_hal_uart.Init.BaudRate/(float)8.0))*MB_RTU_START_CONDITION_CH)
		return 1;	
	return 0;
}
//---------------------------------------------------------------------
uint8_t mb_check_crc16(uint8_t address)
{
	uint8_t crcL,crcH;
	uint16_t crc1,crc2;
	uint8_t size;
	
	size=mb_get_size_pakets(address);
	
	crcH=mb_rx_buffer[address][size-1];
	crcL=mb_rx_buffer[address][size-2];
	
	crc1=CRC16(&mb_rx_buffer[address][0],mb_rx_buffer_size[address]-2);
	crc2=(crcH<<8)|(crcL&0xff);
	
	if(crc1==crc2)return 1;
	else 					return 0;
}
//---------------- mb_uart_set ----------------------------------------
void mb_uart_set(UART_HandleTypeDef *huart)
{
	//huart->Instance = USART2;
  huart->Init.BaudRate = 9600;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
}
//---------------------- mb_tim_set -------------------------------------
void mb_tim_set(TIM_HandleTypeDef *htim)
{

  //htim->Instance = TIM2;
  htim->Init.Prescaler =((HAL_RCC_GetSysClockFreq() / 1000000.0)*100)-1;	// 10Khz freq
  htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim->Init.Period = 0x4000;							//TimeOut
  htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

}
//------------------- mb_tim_init --------------------------------------
void mb_tim_init(TIM_HandleTypeDef *htim)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
	
  if (HAL_TIM_Base_Init(htim) != HAL_OK)
  {
    mb_Error_Handler("HAL_TIM_Base_Init");
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
  {
    mb_Error_Handler("HAL_TIM_ConfigClockSource");
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
  {
    mb_Error_Handler("HAL_TIMEx_MasterConfigSynchronization");
  }

		HAL_TIM_Base_Start_IT(htim);
}
//-------------------- mb_uart_buffer_reset --------------------------
void mb_uart_buffer_reset()
{
	mb_rx_buffer_conter_y=RESET;
	mb_rx_buffer_conter_x=RESET;
	for(int i=0;i<mb_rx_buffer_size_y;i++)mb_rx_buffer_size[i]=RESET;
	MB_UART_STATE=MB_UART_LISTEN;
}
//------------------- mb_uart_init ------------------------------------
void mb_uart_init(UART_HandleTypeDef *huart)
{
if (HAL_UART_Init(huart) != HAL_OK)
  {
    mb_Error_Handler("HAL_UART_Init");
  }
	
	//__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
	mb_uart_reset_char_listen(huart);
	
	//reset buffer
	mb_uart_buffer_reset();
}	
//---------------------- UART intrupt ---------------------------------
void mb_uart_rxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==mb_hal_uart.Instance)
	{
		if(MB_UART_STATE==MB_UART_LISTEN&&mb_check_start()&&	
			#ifdef MB_MASTER
			1
			#else
			MB_UART_BUFF_CH==MB_SLAVE_ID
			#endif		
		)	
		{
			MB_SLAVE_ID=MB_UART_BUFF_CH;
			MB_UART_STATE=MB_UART_MATCH_ID;
			mb_rx_buffer_conter_x=RESET;
			mb_rx_buffer_size[mb_rx_buffer_conter_y]=0;
			mb_rx_buffer[mb_rx_buffer_conter_y][mb_rx_buffer_conter_x]=MB_UART_BUFF_CH;
			mb_rx_buffer_conter_x++;
		}
		else if(MB_UART_STATE==MB_UART_MATCH_ID)
		{
			MB_UART_STATE=MB_UART_READ_FUNCTION;
			MB_FUNCTION=MB_UART_BUFF_CH;
			mb_rx_buffer[mb_rx_buffer_conter_y][mb_rx_buffer_conter_x]=MB_UART_BUFF_CH;
			mb_rx_buffer_conter_x++;
			
			#ifndef MB_MASTER	//Slave
			if(MB_FUNCTION==MB_Read_Coils 						||MB_FUNCTION==MB_Read_Discrete_Inputs ||
				 MB_FUNCTION==MB_Read_Holding_Registers	||MB_FUNCTION==MB_Read_Input_Registers ||
				 MB_FUNCTION==MB_Write_Single_Coil 			||MB_FUNCTION==MB_Write_Single_Register )
			{
				MB_LEN=MB_FREAM_SLAVE_LEN;
			}
			else if(MB_FUNCTION==MB_Write_Multiple_Coils||
							MB_FUNCTION==MB_Write_Multiple_Registers)
			{
				MB_LEN=MB_FREAM_SLAVE_DATA_LEN_LOC;
			}
			else
			{
				MB_UART_STATE=MB_UART_LISTEN;
				mb_rx_buffer_conter_x=RESET;
			}
			#else	//Master
			if(MB_FUNCTION==MB_Read_Coils 						||MB_FUNCTION==MB_Read_Discrete_Inputs ||
				 MB_FUNCTION==MB_Read_Holding_Registers	||MB_FUNCTION==MB_Read_Input_Registers )
			{
				MB_LEN=MB_FREAM_MASTER_DATA_LEN_LOC;
			}
			else if(MB_FUNCTION==MB_Write_Single_Coil 	||MB_FUNCTION==MB_Write_Single_Register||
							MB_FUNCTION==MB_Write_Multiple_Coils||MB_FUNCTION==MB_Write_Multiple_Registers)
			{
				MB_LEN=MB_FREAM_MASTER_LEN;
			}
			else
			{
				MB_UART_STATE=MB_UART_LISTEN;
				mb_rx_buffer_conter_x=RESET;
			}
			#endif				
		}
		else if(MB_UART_STATE==MB_UART_READ_FUNCTION)
		{
			if(mb_rx_buffer_conter_x<=mb_rx_buffer_size_x)
			{
				mb_rx_buffer[mb_rx_buffer_conter_y][mb_rx_buffer_conter_x]=MB_UART_BUFF_CH;
				mb_rx_buffer_conter_x++;
			}
			if(MB_LEN==mb_rx_buffer_conter_x)
			{
				#ifndef MB_MASTER	//Slave
				if(MB_FUNCTION==MB_Read_Coils 						||MB_FUNCTION==MB_Read_Discrete_Inputs ||
					 MB_FUNCTION==MB_Read_Holding_Registers	||MB_FUNCTION==MB_Read_Input_Registers ||
					 MB_FUNCTION==MB_Write_Single_Coil 			||MB_FUNCTION==MB_Write_Single_Register )
				{
					mb_rx_buffer_size[mb_rx_buffer_conter_y]=mb_rx_buffer_conter_x;
					MB_UART_STATE=MB_UART_READY;
				}
				else if(MB_FUNCTION==MB_Write_Multiple_Coils||
								MB_FUNCTION==MB_Write_Multiple_Registers)
				{
					if(mb_rx_buffer_conter_x==MB_FREAM_SLAVE_DATA_LEN_LOC)MB_LEN=MB_LEN+MB_UART_BUFF_CH+2;
					else 
					{
						mb_rx_buffer_size[mb_rx_buffer_conter_y]=mb_rx_buffer_conter_x;
						MB_UART_STATE=MB_UART_READY;
					}
				}
				#else	//Master
				if(MB_FUNCTION==MB_Read_Coils 						||MB_FUNCTION==MB_Read_Discrete_Inputs ||
					 MB_FUNCTION==MB_Read_Holding_Registers	||MB_FUNCTION==MB_Read_Input_Registers )
				{
					if(mb_rx_buffer_conter_x==MB_FREAM_MASTER_DATA_LEN_LOC)MB_LEN=MB_LEN+MB_UART_BUFF_CH+2;
					else 
					{
						mb_rx_buffer_size[mb_rx_buffer_conter_y]=mb_rx_buffer_conter_x;
						MB_UART_STATE=MB_UART_READY;
					}
				}
				else if(MB_FUNCTION==MB_Write_Single_Coil 	||MB_FUNCTION==MB_Write_Single_Register||
								MB_FUNCTION==MB_Write_Multiple_Coils||MB_FUNCTION==MB_Write_Multiple_Registers)
				{
					mb_rx_buffer_size[mb_rx_buffer_conter_y]=mb_rx_buffer_conter_x;
					MB_UART_STATE=MB_UART_READY;
				}
				#endif
			}
		}
		else if(MB_UART_STATE==MB_UART_READY&&mb_check_start()&&
			#ifdef MB_MASTER
			1
			#else
			MB_UART_BUFF_CH==MB_SLAVE_ID
			#endif	
		)
		{
			if(mb_rx_buffer_conter_y<mb_rx_buffer_size_y-1)
			{
				mb_rx_buffer_conter_y++;
				MB_UART_STATE=MB_UART_MATCH_ID;
				mb_rx_buffer_conter_x=RESET;
				mb_rx_buffer[mb_rx_buffer_conter_y][mb_rx_buffer_conter_x]=MB_UART_BUFF_CH;
				mb_rx_buffer_conter_x++;
			}
			else
			{
				MB_UART_STATE=MB_UART_BUFF_IS_FULL;
			}
		}
		
		mb_uart_reset_char_listen(huart);
	}
}
//--------------------------------------------------------
void mb_uart_txCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == mb_hal_uart.Instance)
	{
		mb_pin_tx(DISABLE);
	}
}
//--------------------------------------------------------
void mb_Error_Handler(char *str)
{
	#ifdef MB_DEBUG
	HAL_UART_Transmit(&mb_hal_uart,(uint8_t *)"\r\nERROR : ",10,100);
	HAL_UART_Transmit(&mb_hal_uart,(uint8_t *)str,strlen(str),100);
	HAL_UART_Transmit(&mb_hal_uart,(uint8_t *)"\r\nStoping..",11,100);
	while(1)
	{
		HAL_UART_Transmit(&mb_hal_uart,(uint8_t *)".",1,100);
		HAL_Delay(2000);
	}
	#endif
}
//--------------------------------------------------------
void mb_log(char *str)
{
	#ifdef MB_DEBUG	
	do{
	HStatus=HAL_UART_Transmit_IT(&mb_hal_uart,(uint8_t *)str,strlen(str));
	}while(HStatus!=HAL_OK);
	#endif
}
//---------------------------------------------------------
//CRC16
uint16_t CRC16 (const uint8_t *nData, uint16_t wLength)
{
	static const uint16_t wCRCTable[] = {
	0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
	};
	uint8_t nTemp;
	uint16_t wCRCWord = 0xFFFF;
	while (wLength--)
	{
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];
	}
	return wCRCWord;
}
//--------------------------------------------------------------------
void mb_pin_tx(uint8_t ENABLE_or_DISABLE)
{
	if(ENABLE_or_DISABLE)
	{
		mb_pin_rx(DISABLE);
		HAL_GPIO_WritePin(mb_gpio_txe_port, mb_gpio_txe_pin, GPIO_PIN_SET);		
	}
	else
	{
		HAL_GPIO_WritePin(mb_gpio_txe_port, mb_gpio_txe_pin, GPIO_PIN_RESET);
		mb_pin_rx(ENABLE);
	}
		
}
//--------------------------------------------------------------------
void mb_pin_rx(uint8_t ENABLE_or_DISABLE)
{
	if(ENABLE_or_DISABLE)
  HAL_GPIO_WritePin(mb_gpio_rxe_port, mb_gpio_rxe_pin, GPIO_PIN_RESET);
	else
	HAL_GPIO_WritePin(mb_gpio_rxe_port, mb_gpio_rxe_pin, GPIO_PIN_SET);
		
}
//--------------------------------------------------------------------
void mb_init(uint8_t mb_mode,UART_HandleTypeDef *huart,TIM_HandleTypeDef *htim)
{
	switch(mb_mode)
	{
		case MB_MODE_RTU:
			mb_pin_tx(DISABLE);
			mb_uart_set(huart);
			mb_uart_init(huart);
			mb_tim_set(htim);
			mb_tim_init(htim);
		
		break;	
		
		case MB_MODE_ASCII:
			mb_Error_Handler("MB_MODE_ASCII not set! (it coming soon)");	
		break;	
		
		case MB_MODE_TCP:
			mb_Error_Handler("MB_MODE_TCP not set! (it coming soon)");	
		break;	
		
		default:mb_Error_Handler("MB_MODE not match!");	
	}
	
}
//--------------------------------------------------------------------
uint8_t mb_rtu_request_read_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Coils;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//--------------------------------------------------------------------------------------
uint8_t mb_rtu_response_read_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Coils;	
	mb_tx_buffer[2]= len;
	
	for(i=3,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
		
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	return 1;
}
//-------------------------------------------------------------------------------------
uint8_t mb_rtu_request_read_discrete_inputs(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Discrete_Inputs;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_response_read_discrete_inputs(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Discrete_Inputs;	
	mb_tx_buffer[2]= len;
	
	for(i=3,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
		
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	return 1;
}
//-------------------------------------------------------------------------------------
uint8_t mb_rtu_request_read_holding_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Holding_Registers;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_response_read_holding_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Holding_Registers;
	mb_tx_buffer[2]= len;
	
	for(i=3,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
		
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	return 1;
}
//-------------------------------------------------------------------------------------
uint8_t mb_rtu_request_read_input_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Input_Registers;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_response_read_input_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Read_Input_Registers;	
	mb_tx_buffer[2]= len;
	
	for(i=3,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
		
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;	

	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	return 1;
}
//--------------------------------------------------------------------------------
uint8_t mb_rtu_request_write_single_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Single_Coil	;
	mb_tx_buffer[2]=(CAddress>>8)&0xff;
	mb_tx_buffer[3]= CAddress&0xff;
	mb_tx_buffer[4]=(Data>>8)&0xff;
	mb_tx_buffer[5]= Data&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_response_write_single_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Single_Coil	;
	mb_tx_buffer[2]=(CAddress>>8)&0xff;
	mb_tx_buffer[3]= CAddress&0xff;
	mb_tx_buffer[4]=(Data>>8)&0xff;
	mb_tx_buffer[5]= Data&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	
	return 1;
}
//--------------------------------------------------------------------------------
uint8_t mb_rtu_request_write_single_register(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Single_Register	;
	mb_tx_buffer[2]=(RAddress>>8)&0xff;
	mb_tx_buffer[3]= RAddress&0xff;
	mb_tx_buffer[4]=(Data>>8)&0xff;
	mb_tx_buffer[5]= Data&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_response_write_single_register(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Single_Register	;
	mb_tx_buffer[2]=(RAddress>>8)&0xff;
	mb_tx_buffer[3]= RAddress&0xff;
	mb_tx_buffer[4]=(Data>>8)&0xff;
	mb_tx_buffer[5]= Data&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	
	return 1;
}
//--------------------------------------------------------------------------------
uint8_t mb_rtu_request_write_multiple_coils(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Multiple_Coils;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	mb_tx_buffer[6]= len;
	
	for(i=7,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
	
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//-----------------------------------------------------------------------------------
uint8_t mb_rtu_response_write_multiple_coils(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Single_Register	;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	
	return 1;
}
//------------------------------------------------------------------------------------
uint8_t mb_rtu_request_write_multiple_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *DATA)
{
	uint16_t crc;
	uint8_t LEN,i,j;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Multiple_Registers;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	mb_tx_buffer[6]= len;
	
	for(i=7,j=0;j<len;i++,j++)
	{
		mb_tx_buffer[i]=DATA[j];
	}
	
	crc=CRC16(mb_tx_buffer,i);
	
	
	mb_tx_buffer[i]= crc&0xff;
	i++;
	mb_tx_buffer[i]=(crc>>8)&0xff;
	
	LEN=i+1;
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	HAL_Delay(MB_SEND_PACKET_DELAY_MS);
	return 1;
}
//-----------------------------------------------------------------------------------
uint8_t mb_rtu_response_write_multiple_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	uint16_t crc;
	uint8_t LEN=8;
	
	mb_tx_buffer[0]= SlaveAd;
	mb_tx_buffer[1]= MB_Write_Multiple_Registers	;
	mb_tx_buffer[2]=(StartAd>>8)&0xff;
	mb_tx_buffer[3]= StartAd&0xff;
	mb_tx_buffer[4]=(Quantity>>8)&0xff;
	mb_tx_buffer[5]= Quantity&0xff;
	 
	crc=CRC16(mb_tx_buffer,6);
	
	mb_tx_buffer[6]= crc&0xff;
	mb_tx_buffer[7]=(crc>>8)&0xff;
	
	
	while(HAL_GPIO_ReadPin(mb_gpio_txe_port,mb_gpio_txe_pin)==GPIO_PIN_SET);
	
	mb_pin_tx(ENABLE);	
	do{
	HStatus=HAL_UART_Transmit_IT(huart,mb_tx_buffer,LEN);		
	}while(HStatus!=HAL_OK);
	
	
	return 1;
}
//------------------------------------------------------------------------------------
