#ifndef _MB_H_
#define _MB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Mode List
#define MB_MODE_MASTER  0
#define MB_MODE_SLAVE   1

// Mode Select
#define MB_MODE         MB_MODE_SLAVE

// Default SLAVE mode Device Address
#define MB_DEFAULT_SLAVE_ADDRESS 0x01

// Comment that for release
//#define MB_DEBUG

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

typedef enum{
	MB_PACKET_TYPE_UNKNOWN,
	MB_PACKET_TYPE_ERROR,
	MB_PACKET_TYPE_Slave_Responce_Var,
	MB_PACKET_TYPE_Slave_Responce_Fix,
	MB_PACKET_TYPE_Master_Request_Fix,
	MB_PACKET_TYPE_Master_Request_Var,
}mb_packet_type_e;

typedef struct{
	uint8_t device_address;
	uint8_t func;
	uint16_t u16_1;
	uint16_t u16_2;
	uint8_t len;
	uint8_t err;
	uint8_t *Data;
	mb_packet_type_e type;
}mb_packet_s;


typedef struct{
	uint8_t address;
	void (*tx_handler)(uint8_t *,uint8_t);
	#if(MB_MODE==MB_MODE_MASTER)
	void (*master_process_handler)(mb_packet_s);
	#endif
}mb_config_s;

extern mb_config_s MB_Config;

#if(MB_MODE==MB_MODE_SLAVE)
void             mb_slave_address_set(uint8_t Address);
uint8_t          mb_slave_address_get(void);
#elif(MB_MODE==MB_MODE_MASTER)
void             mb_set_master_process_handler(void (f)(mb_packet_s));
#endif
void             mb_set_tx_handler(void (f)(uint8_t *,uint8_t));

void             mb_rx_new_data(uint8_t Byte);
void             mb_rx_timeout_handler(void);

void             mb_tx_packet_handler(mb_packet_s Packet);
void             mb_rx_packet_handler(mb_packet_s Packet);

void             mb_error_handler(mb_functions_e func,mb_error_e err);

#ifdef __cplusplus
}
#endif

#endif
