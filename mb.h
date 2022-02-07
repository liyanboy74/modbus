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

//MODBUS Exception Codes
typedef enum{
	MB_OK = 0x00,
	MB_ERROR_ILLEGAL_FUNCTION = 0x01,
	MB_ERROR_ILLEGAL_DATA_ADDRESS = 0x02,
	MB_ERROR_ILLEGAL_DATA_VALUE = 0x03,
	MB_ERROR_SLAVE_DEVICE_FAILURE = 0x04,
	MB_ERROR_ACKNOWLEDGE = 0x05,
	MB_ERROR_SLAVE_DEVICE_BUSY = 0x06,
	MB_ERROR_MEMORY_PARITY = 0x08,
	MB_ERROR_GATEWAY_PATH_UNAVAILABLE = 0x0A,
	MB_ERROR_FAILED_TO_RESPOND = 0x0B,
}mb_error_e;

typedef struct {
	uint8_t *request;
	uint8_t *responce;
	uint8_t request_len;
	uint8_t responce_len;
	mb_error_e err;
	mb_functions_e func;
}mb_process_s;

mb_error_e mb_check_func(uint8_t func);
mb_error_e mb_check_quantity(uint16_t Quantity);
mb_error_e mb_check_table_bit_address(uint16_t StartAd,uint16_t Quantity);
mb_error_e mb_check_table_address(uint16_t StartAd,uint16_t Quantity);


#endif