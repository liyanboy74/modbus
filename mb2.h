#ifndef _MB_H_
#define _MB_H_

#include <stdint.h>

//ModBus Fanctions
typedef enum{
	MB_Read_Coils					= 0x01,
	MB_Read_Discrete_Inputs			= 0x02,
	MB_Read_Holding_Registers		= 0x03,
	MB_Read_Input_Registers			= 0x04,
	MB_Write_Single_Coil			= 0x05,
	MB_Write_Single_Register		= 0x06,
	MB_Write_Multiple_Coils			= 0x0f,
	MB_Write_Multiple_Registers		= 0x10,
}mb_functions_e;

uint8_t mb_rtu_request_read_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_response_read_coil(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data);

uint8_t mb_rtu_request_read_discrete_inputs(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_response_read_discrete_inputs(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data);

uint8_t mb_rtu_request_read_holding_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_response_read_holding_registers(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data);

uint8_t mb_rtu_request_read_input_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_response_read_input_registers(uint8_t *Buffer,uint8_t SlaveAd,uint8_t len,uint8_t *Data);

uint8_t mb_rtu_request_write_single_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data);
uint8_t mb_rtu_response_write_single_coil(uint8_t *Buffer,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data);

uint8_t mb_rtu_request_write_single_register(uint8_t *Buffer,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data);
uint8_t mb_rtu_response_write_single_register(uint8_t *Buffer,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data);

uint8_t mb_rtu_request_write_multiple_coils(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
uint8_t mb_rtu_response_write_multiple_coils(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);

uint8_t mb_rtu_request_write_multiple_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
uint8_t mb_rtu_response_write_multiple_registers(uint8_t *Buffer,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);

#endif
