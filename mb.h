/*
 MODBUS
 By Liyanboy74
 https://github.com/liyanboy74/modbus
*/

#ifndef _MB_H_
#define _MB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Comment that for release
//#define MB_DEBUG

// Mode List
#define MB_MODE_MASTER  0
#define MB_MODE_SLAVE   1

// Mode Select
#ifndef MB_MODE
	#define MB_MODE         MB_MODE_SLAVE
#endif

// Default SLAVE mode Device Address
#define MB_DEFAULT_SLAVE_ADDRESS 		0x01

// Listen slave address MB_BROADCAST_ADDRESS but don't response to master
// Comment that for disable support broadcast packets
#define MB_SLAVE_LISTEN_BROADCAST

// Default Broadcast address
#define MB_BROADCAST_ADDRESS 0

// Enable or Disable Support Modbus Function for Slave Mode
#define MB_ENABLE_FUNC_Read_Coils               1
#define MB_ENABLE_FUNC_Read_Discrete_Inputs     1
#define MB_ENABLE_FUNC_Read_Holding_Registers   1
#define MB_ENABLE_FUNC_Read_Input_Registers     1
#define MB_ENABLE_FUNC_Write_Single_Coil        1
#define MB_ENABLE_FUNC_Write_Single_Register    1
#define MB_ENABLE_FUNC_Write_Multiple_Coils     1
#define MB_ENABLE_FUNC_Write_Multiple_Registers 1

#define MB_COIL_ON  0xff00
#define MB_COIL_OFF 0x0000

#define MB_MIN_QUANTITY 0x0001
#define MB_MAX_QUANTITY 0x07d0

// ModBus Fanctions
typedef enum{
	MB_FUNC_Read_Coils					= 0x01,
	MB_FUNC_Read_Discrete_Inputs		= 0x02,
	MB_FUNC_Read_Holding_Registers		= 0x03,
	MB_FUNC_Read_Input_Registers		= 0x04,
	MB_FUNC_Write_Single_Coil			= 0x05,
	MB_FUNC_Write_Single_Register		= 0x06,
	MB_FUNC_Write_Multiple_Coils		= 0x0f,
	MB_FUNC_Write_Multiple_Registers	= 0x10
}mb_functions_e;

// MODBUS Exception Codes
typedef enum{
	MB_OK = 0x00,
	MB_ERROR_ILLEGAL_FUNCTION 			= 0x01,
	MB_ERROR_ILLEGAL_DATA_ADDRESS 		= 0x02,
	MB_ERROR_ILLEGAL_DATA_VALUE 		= 0x03,
	MB_ERROR_SLAVE_DEVICE_FAILURE 		= 0x04,
	MB_ERROR_ACKNOWLEDGE 				= 0x05,
	MB_ERROR_SLAVE_DEVICE_BUSY 			= 0x06,
	MB_ERROR_MEMORY_PARITY 				= 0x08,
	MB_ERROR_GATEWAY_PATH_UNAVAILABLE 	= 0x0A,
	MB_ERROR_FAILED_TO_RESPOND 			= 0x0B
}mb_error_e;

// MODBUS Packet Type
typedef enum{
	MB_PACKET_TYPE_UNKNOWN,
	MB_PACKET_TYPE_ERROR,
	MB_PACKET_TYPE_Slave_Responce_Var,
	MB_PACKET_TYPE_Slave_Responce_Fix,
	MB_PACKET_TYPE_Master_Request_Fix,
	MB_PACKET_TYPE_Master_Request_Var
}mb_packet_type_e;

// MODBUS Packet Struct
typedef struct{
	uint8_t device_address;	// Slave Device Address

	uint8_t func;			// MODBUS Func Code
	mb_packet_type_e type;	// Packet Type = (Fix,Var,Error) ref to 'mb_packet_type_e'

	uint16_t u16_1;			// Data1 if Packet Type = Fixed Size
	uint16_t u16_2;			// Data2 if Packet Type = Fixed Size

	uint8_t len;			// Data Len if Packet Type = Var Size
	uint8_t *Data;			// Data     if Packet Type = Var Size

	uint8_t err;			// Error Value if Packet Type = Error
}mb_packet_s;

// MODBUS Config
typedef struct{
	uint8_t address;
	void (*tx_handler)(uint8_t *,uint8_t);
	#if(MB_MODE==MB_MODE_MASTER)
	void (*master_process_handler)(mb_packet_s);
	#endif
}mb_config_s;

// Set MODBUS Config Public
extern mb_config_s MB_Config;

// If Defined Mode as Slave
#if(MB_MODE==MB_MODE_SLAVE)

void             mb_slave_address_set(uint8_t Address);						// Set Slave Address
uint8_t          mb_slave_address_get(void);								// Get Slave Address

void             mb_error_handler(mb_packet_s* Packet,mb_error_e err);		// Make and Send ERROR Packet in Slave Mode

// If Defined Mode as Master
#elif(MB_MODE==MB_MODE_MASTER)

void             mb_set_master_process_handler(void (*f)(mb_packet_s));		// Set Callback fot Process Received Packet as Master

#endif

void             mb_set_tx_handler(void (*f)(uint8_t *,uint8_t));			// Set Callback for Transmit Data as Master or Slave

void             mb_rx_new_data(uint8_t Byte);								// Get New Data for Detect Valid MODBUS Data
void             mb_rx_timeout_handler(void);								// Reset MODBUS Input Buffer Index as Timeout Error

void             mb_tx_packet_handler(mb_packet_s Packet);					// Prepare and Send Packet
void             mb_rx_packet_handler(mb_packet_s Packet);					// Start Processing Received Packet


#ifdef __cplusplus
}
#endif

#endif
