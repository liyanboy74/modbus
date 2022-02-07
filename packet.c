#include "mb.h"
#include "packet.h"

mb_packet_len mb_rtu_request_read_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Coils;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_read_coil(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Coils;
	Buffer[2]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+3]=Data[i];
	}

	return i+3;
}

mb_packet_len mb_rtu_request_read_discrete_inputs(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Discrete_Inputs;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_read_discrete_inputs(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Discrete_Inputs;
	Buffer[2]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+3]=Data[i];
	}

	return i+3;
}

mb_packet_len mb_rtu_request_read_holding_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Holding_Registers;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_read_holding_registers(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Holding_Registers;
	Buffer[2]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+3]=Data[i];
	}

	return i+3;
}

mb_packet_len mb_rtu_request_read_input_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Input_Registers;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_read_input_registers(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Read_Input_Registers;
	Buffer[2]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+3]=Data[i];
	}

	return i+3;
}

mb_packet_len mb_rtu_request_write_single_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Single_Coil	;
	Buffer[2]=(CAddress>>8)&0xff;
	Buffer[3]= CAddress&0xff;
	Buffer[4]=(Data>>8)&0xff;
	Buffer[5]= Data&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_write_single_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Single_Coil;
	Buffer[2]=(CAddress>>8)&0xff;
	Buffer[3]= CAddress&0xff;
	Buffer[4]=(Data>>8)&0xff;
	Buffer[5]= Data&0xff;

	return 6;
}

mb_packet_len mb_rtu_request_write_single_register(uint8_t *Buffer,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Single_Register	;
	Buffer[2]=(RAddress>>8)&0xff;
	Buffer[3]= RAddress&0xff;
	Buffer[4]=(Data>>8)&0xff;
	Buffer[5]= Data&0xff;

	return 6;
}

mb_packet_len mb_rtu_response_write_single_register(uint8_t *Buffer,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Single_Register	;
	Buffer[2]=(RAddress>>8)&0xff;
	Buffer[3]= RAddress&0xff;
	Buffer[4]=(Data>>8)&0xff;
	Buffer[5]= Data&0xff;

	return 6;
}

mb_packet_len mb_rtu_request_write_multiple_coils(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Multiple_Coils;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;
	Buffer[6]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+7]=Data[i];
	}

	return i+7;
}

mb_packet_len mb_rtu_response_write_multiple_coils(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Single_Register	;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_request_write_multiple_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	uint8_t i;

	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Multiple_Registers;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;
	Buffer[6]= len;

	for(i=0;i<len;i++)
	{
		Buffer[i+7]=Data[i];
	}

	return i+7;
}

mb_packet_len mb_rtu_response_write_multiple_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity)
{
	Buffer[0]= SlaveAd;
	Buffer[1]= MB_Write_Multiple_Registers	;
	Buffer[2]=(StartAd>>8)&0xff;
	Buffer[3]= StartAd&0xff;
	Buffer[4]=(Quantity>>8)&0xff;
	Buffer[5]= Quantity&0xff;

	return 6;
}

mb_packet_len mb_rtu_error(uint8_t *Buffer,mb_functions_e Func,mb_error_e Exeption_Code)
{
	Buffer[0]= 0x80|Func;
	Buffer[1]= Exeption_Code;

	return 2;
}
