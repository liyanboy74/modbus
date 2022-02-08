#ifndef _MB_H_
#define _MB_H_

#include <stdint.h>

// Comment that for release
#define MB_DEBUG

// ModBus Fanctions
typedef enum{
	MB_FUNC_Read_Coils					= 0x01,
	MB_FUNC_Read_Discrete_Inputs			= 0x02,
	MB_FUNC_Read_Holding_Registers		= 0x03,
	MB_FUNC_Read_Input_Registers			= 0x04,
	MB_FUNC_Write_Single_Coil			= 0x05,
	MB_FUNC_Write_Single_Register		= 0x06,
	MB_FUNC_Write_Multiple_Coils			= 0x0f,
	MB_FUNC_Write_Multiple_Registers		= 0x10,
}mb_functions_e;

// MODBUS Exception Codes
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

// MOSBUS Mode
typedef enum{
	MB_MODE_MASTER=0,
	MB_MODE_SLAVE=1,
}mb_mode_e;

typedef enum{
	MB_PACKET_Master_Responce_Var,
	MB_PACKET_Master_Responce_Fix,
	MB_PACKET_Slave_Responce_Var,
	MB_PACKET_Slave_Responce_Fix,
}mb_packet_type_e;

typedef struct{
	mb_mode_e mode;
	uint8_t address;
}mb_config_s;

typedef struct{
	uint8_t device_address;
	uint8_t func;
	uint16_t u16_1;
	uint16_t u16_2;
	mb_packet_type_e type;
	uint8_t len;
	uint8_t *Data;
}mb_packet_s;

void        mb_mode_set(mb_mode_e Mode);
mb_mode_e   mb_mode_get(void);

void        mb_slave_address_set(uint8_t Address);
uint8_t     mb_slave_address_get(void);

mb_packet_s mb_rx_packet_split(uint8_t *Packet_Buffer,uint8_t Packet_Len,mb_packet_type_e Packet_Type);
void        mb_rx_packet_handler(mb_packet_s Packet);

void        mb_error_handler(mb_error_e err);


mb_error_e mb_slave_process_read_coils(mb_packet_s Packet);
mb_error_e mb_slave_process_read_discrere_inputs(mb_packet_s Packet);
mb_error_e mb_slave_process_read_holding_registers(mb_packet_s Packet);
mb_error_e mb_slave_process_read_input_registers(mb_packet_s Packet);


#endif
