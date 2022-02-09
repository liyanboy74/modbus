#include "mb-packet.h"

mb_packet_s mb_packet_request_read_coil(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.func= MB_FUNC_Read_Coils;
	Packet.type=MB_PACKET_TYPE_Master_Request_Fix;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_response_read_coil(uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Var;
	Packet.func= MB_FUNC_Read_Coils;
	Packet.len= len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_request_read_discrete_inputs(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Fix;
	Packet.func= MB_FUNC_Read_Discrete_Inputs;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_response_read_discrete_inputs(uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Var;
	Packet.func= MB_FUNC_Read_Discrete_Inputs;
	Packet.len= len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_request_read_holding_registers(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Fix;
	Packet.func= MB_FUNC_Read_Holding_Registers;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_response_read_holding_registers(uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Var;
	Packet.func= MB_FUNC_Read_Holding_Registers;
	Packet.len = len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_request_read_input_registers(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Fix;
	Packet.func= MB_FUNC_Read_Input_Registers;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_response_read_input_registers(uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Var;
	Packet.func= MB_FUNC_Read_Input_Registers;
	Packet.len= len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_request_write_single_coil(uint16_t CAddress,uint16_t Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Fix;
	Packet.func= MB_FUNC_Write_Single_Coil;
	Packet.u16_1=CAddress;
	Packet.u16_2=Data;
	return Packet;
}

mb_packet_s mb_packet_response_write_single_coil(uint16_t CAddress,uint16_t Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Fix;
	Packet.func= MB_FUNC_Write_Single_Coil;
	Packet.u16_1=CAddress;
	Packet.u16_2=Data;
	return Packet;
}

mb_packet_s mb_packet_request_write_single_register(uint16_t RAddress,uint16_t Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Fix;
	Packet.func= MB_FUNC_Write_Single_Register;
	Packet.u16_1=RAddress;
	Packet.u16_2=Data;
	return Packet;
}

mb_packet_s mb_packet_response_write_single_register(uint16_t RAddress,uint16_t Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Fix;
	Packet.func= MB_FUNC_Write_Single_Register;
	Packet.u16_1=RAddress;
	Packet.u16_2=Data;
	return Packet;
}

mb_packet_s mb_packet_request_write_multiple_coils(uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Var;
	Packet.func= MB_FUNC_Write_Multiple_Coils;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	Packet.len= len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_response_write_multiple_coils(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Fix;
	Packet.func= MB_FUNC_Write_Multiple_Coils;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_request_write_multiple_registers(uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Master_Request_Var;
	Packet.func= MB_FUNC_Write_Multiple_Registers;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	Packet.len=len;
	Packet.Data=Data;
	return Packet;
}

mb_packet_s mb_packet_response_write_multiple_registers(uint16_t StartAd,uint16_t Quantity)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_Slave_Responce_Fix;
	Packet.func= MB_FUNC_Write_Multiple_Registers;
	Packet.u16_1=StartAd;
	Packet.u16_2=Quantity;
	return Packet;
}

mb_packet_s mb_packet_error(mb_functions_e Func,mb_error_e Exeption_Code)
{
	mb_packet_s Packet;
	Packet.type=MB_PACKET_TYPE_ERROR;
	Packet.func= 0x80|Func;
	Packet.len= Exeption_Code;
	return Packet;
}
