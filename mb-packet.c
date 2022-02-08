#include "mb-packet.h"

uint8_t mb_rtu_request_read_coil(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Read_Coils;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_response_read_coil(uint8_t *Buffer,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Read_Coils;
	Buffer[1]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+2]=Data[i];
	}

	return i+2;
}

uint8_t mb_rtu_request_read_discrete_inputs(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Read_Discrete_Inputs;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_response_read_discrete_inputs(uint8_t *Buffer,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Read_Discrete_Inputs;
	Buffer[1]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+2]=Data[i];
	}

	return i+2;
}

uint8_t mb_rtu_request_read_holding_registers(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Read_Holding_Registers;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_response_read_holding_registers(uint8_t *Buffer,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Read_Holding_Registers;
	Buffer[1]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+2]=Data[i];
	}

	return i+2;
}

uint8_t mb_rtu_request_read_input_registers(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Read_Input_Registers;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_response_read_input_registers(uint8_t *Buffer,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Read_Input_Registers;
	Buffer[1]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+2]=Data[i];
	}

	return i+2;
}

uint8_t mb_rtu_request_write_single_coil(uint8_t *Buffer,uint16_t CAddress,uint16_t Data)
{
	Buffer[0]= MB_FUNC_Write_Single_Coil	;
	Buffer[1]=(CAddress>>8)&0xff;
	Buffer[2]= CAddress&0xff;
	Buffer[3]=(Data>>8)&0xff;
	Buffer[4]= Data&0xff;

	return 5;
}

uint8_t mb_rtu_response_write_single_coil(uint8_t *Buffer,uint16_t CAddress,uint16_t Data)
{
	return mb_rtu_request_write_single_coil(Buffer,CAddress,Data);
}

uint8_t mb_rtu_request_write_single_register(uint8_t *Buffer,uint16_t RAddress,uint16_t Data)
{
	Buffer[0]= MB_FUNC_Write_Single_Register	;
	Buffer[1]=(RAddress>>8)&0xff;
	Buffer[2]= RAddress&0xff;
	Buffer[3]=(Data>>8)&0xff;
	Buffer[4]= Data&0xff;

	return 5;
}

uint8_t mb_rtu_response_write_single_register(uint8_t *Buffer,uint16_t RAddress,uint16_t Data)
{
	return mb_rtu_request_write_single_register(Buffer,RAddress,Data);
}

uint8_t mb_rtu_request_write_multiple_coils(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Write_Multiple_Coils;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;
	Buffer[5]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+6]=Data[i];
	}

	return i+6;
}

uint8_t mb_rtu_response_write_multiple_coils(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Write_Multiple_Coils	;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_request_write_multiple_registers(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= MB_FUNC_Write_Multiple_Registers;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;
	Buffer[5]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+6]=Data[i];
	}

	return i+6;
}

uint8_t mb_rtu_response_write_multiple_registers(uint8_t *Buffer,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= MB_FUNC_Write_Multiple_Registers	;
	Buffer[1]=(StartAd>>8)&0xff;
	Buffer[2]= StartAd&0xff;
	Buffer[3]=(Quantity>>8)&0xff;
	Buffer[4]= Quantity&0xff;

	return 5;
}

uint8_t mb_rtu_error(uint8_t *Buffer,mb_functions_e Func,mb_error_e Exeption_Code)
{
	Buffer[0]= 0x80|Func;
	Buffer[1]= Exeption_Code;

	return 2;
}
