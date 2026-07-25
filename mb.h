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
// #define MB_DEBUG

// Mode List
#define MB_MODE_MASTER  0
#define MB_MODE_SLAVE   1


// Mode Select
#ifndef MB_MODE
	#define MB_MODE         MB_MODE_SLAVE
#endif

// Default SLAVE mode Device Address
#define MB_DEFAULT_SLAVE_ADDRESS 		0x01

#define MB_DEVICE_IDENTIFICATION_VENDOR                 "VENDOR"
#define MB_DEVICE_IDENTIFICATION_PRODUCT                "PRODUCT"
#define MB_DEVICE_IDENTIFICATION_REVISION               "1.0"

// Listen slave address MB_BROADCAST_ADDRESS but don't response to master
// Comment that for disable support broadcast packets
#define MB_SLAVE_LISTEN_BROADCAST

// Default Broadcast address
#define MB_BROADCAST_ADDRESS 0

// Default All layers buffer size
#define MB_DEFAULT_BUFFER_SIZE 32

// Enable or Disable Support Modbus Function for Slave Mode
#define MB_ENABLE_FUNC_Read_Coils                       1
#define MB_ENABLE_FUNC_Read_Discrete_Inputs             1
#define MB_ENABLE_FUNC_Read_Holding_Registers           1
#define MB_ENABLE_FUNC_Read_Input_Registers             1
#define MB_ENABLE_FUNC_Write_Single_Coil                1
#define MB_ENABLE_FUNC_Write_Single_Register            1
#define MB_ENABLE_FUNC_Write_Multiple_Coils             1
#define MB_ENABLE_FUNC_Write_Multiple_Registers         1
#define MB_ENABLE_FUNC_Read_Write_Multiple_Registers    0
#define MB_ENABLE_FUNC_Read_Exception_Status            1
#define MB_ENABLE_FUNC_Encapsulated_Interface           1


#define MB_COIL_ON  0xff00
#define MB_COIL_OFF 0x0000


#define MB_U16_AT(ptr) ((uint16_t)(((uint16_t)((ptr)[0]) << 8) | ((ptr)[1])))

// ModBus Fanctions
typedef enum{
    MB_FUNC_Read_Coils                  = 0x01,
    MB_FUNC_Read_Discrete_Inputs        = 0x02,
    MB_FUNC_Read_Holding_Registers      = 0x03,
    MB_FUNC_Read_Input_Registers        = 0x04,
    MB_FUNC_Write_Single_Coil           = 0x05,
    MB_FUNC_Write_Single_Register       = 0x06,
    MB_FUNC_Read_Exception_Status       = 0x07,

    MB_FUNC_Diagnostics                 = 0x08,
    MB_FUNC_Get_Comm_Event_Counter      = 0x0B,
    MB_FUNC_Get_Comm_Event_Log          = 0x0C,

    MB_FUNC_Write_Multiple_Coils        = 0x0F,
    MB_FUNC_Write_Multiple_Registers    = 0x10,

    MB_FUNC_Report_Server_ID            = 0x11,

    MB_FUNC_Read_File_Record            = 0x14,
    MB_FUNC_Write_File_Record           = 0x15,
    MB_FUNC_Mask_Write_Register         = 0x16,
    MB_FUNC_Read_Write_Multiple_Registers = 0x17,
    MB_FUNC_Read_FIFO_Queue             = 0x18,

    MB_FUNC_Encapsulated_Interface      = 0x2B

} mb_function_e;


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

// MODBUS Packet Struct
typedef struct
{
    uint8_t unit_id;
    mb_function_e function;
    uint16_t length;
    uint8_t *payload;
}mb_packet_s;


// MODBUS Config
typedef struct{
	uint8_t address;
    uint8_t status;
	void (*tx_handler)(uint8_t *,uint16_t);
	void (*master_process_handler)(mb_packet_s);
}mb_config_s;

// Set MODBUS Config Public
extern mb_config_s mb;

// If Defined Mode as Slave
#if(MB_MODE==MB_MODE_SLAVE)

void            mb_slave_address_set(uint8_t Address);						// Set Slave Address
uint8_t         mb_slave_address_get(void);                                 // Get Slave Address

void            mb_slave_status_set(uint8_t status);						// Set Slave status
uint8_t         mb_slave_status_get(void);								    // Get Slave status

void            mb_error_handler(mb_packet_s* Packet,mb_error_e err);		// Make and Send ERROR Packet in Slave Mode

// If Defined Mode as Master
#elif(MB_MODE==MB_MODE_MASTER)

void            mb_set_master_process_handler(void (*f)(mb_packet_s));		// Set Callback fot Process Received Packet as Master

#endif

void            mb_set_tx_handler(void (*f)(uint8_t *,uint16_t));			// Set Callback for Transmit Data as Master or Slave

void            mb_rx_packet_handler(mb_packet_s Packet);					// Start Processing Received Packet
void            mb_tx_packet_handler(mb_packet_s Packet);					// Prepare and Send Packet


#ifdef __cplusplus
}
#endif

#endif
