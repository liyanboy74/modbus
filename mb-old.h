/*
 ModBus For Stm32
 By Liyanboy74
 https://github.com/liyanboy74
 
 More Info: https://simplymodbus.ca 
*/


#include "main.h"

//-------------------- User Setting -----------------------------

//Slave ID
#define MB_Def_Slave_ID		0x01

//comment that for realise
//#define MB_DEBUG

//comment that for slave
#define MB_MASTER

//buffer size
#define mb_tx_buffer_size 	25
#define mb_rx_buffer_size_x 25
#define mb_rx_buffer_size_y 5

//Tx Enable pin
#define mb_gpio_txe_pin  	RS485_DE_Pin
#define mb_gpio_txe_port 	RS485_DE_GPIO_Port

//Rx /Enable pin 
#define mb_gpio_rxe_pin  	RS485_RE_Pin
#define mb_gpio_rxe_port 	RS485_RE_GPIO_Port

//Connect to hal
#define mb_hal_uart huart3
#define mb_hal_tim 	htim2

#define MB_RTU_START_CONDITION_CH 3.5
#define MB_SEND_PACKET_DELAY_MS 10
/*************************************************************************/
//UART STATUS TYPEDEF
typedef enum
{
  MB_UART_LISTEN=0,
	MB_UART_MATCH_ID,
	MB_UART_READ_FUNCTION,
	MB_UART_READY,
	MB_UART_BUFF_IS_FULL,
} MB_UART_STATUS_typedef;

//hal uart HandleTypeDef
extern UART_HandleTypeDef mb_hal_uart;

//hal tim HandleTypeDef
extern TIM_HandleTypeDef 	mb_hal_tim;

//Mod Bus Param
extern uint8_t MB_SLAVE_ID;
extern MB_UART_STATUS_typedef MB_UART_STATE;

//buffer
extern uint8_t mb_tx_buffer[mb_tx_buffer_size];
extern uint8_t mb_rx_buffer[mb_rx_buffer_size_y][mb_rx_buffer_size_x];

//ModBus Fanctions
#define MB_Read_Coils					0x01
#define MB_Read_Discrete_Inputs			0x02
#define MB_Read_Holding_Registers		0x03
#define MB_Read_Input_Registers			0x04
#define MB_Write_Single_Coil			0x05
#define MB_Write_Single_Register		0x06
#define MB_Write_Multiple_Coils			0x0f
#define MB_Write_Multiple_Registers		0x10


//MB Mode
#define MB_MODE_ASCII						0
#define MB_MODE_RTU 						1
#define MB_MODE_TCP							2

//MB Bit Defines
#define MB_BIT_SLAVE_ID 					0
#define MB_BIT_FUNCTION 					1
#define MB_BIT_DATA_LEN 					2
#define MB_BIT_START_DATA					3

//MB Fream Defines
#define MB_FREAM_MASTER_DATA_LEN_LOC		3
#define MB_FREAM_MASTER_LEN					8
#define MB_FREAM_SLAVE_DATA_LEN_LOC			7
#define MB_FREAM_SLAVE_LEN					8

#define RESET								0
#define SET									1

//#define DISABLE	0
//#define ENABLE	1 

/*************************************************************************/
void mb_uart_rxCpltCallback(UART_HandleTypeDef *huart);
void mb_uart_txCpltCallback(UART_HandleTypeDef *huart);
void mb_tim_periodElapsedCallback(TIM_HandleTypeDef *htim);
void mb_Error_Handler(char *str);
void mb_log(char *str);
void mb_dir_set(void);
void mb_pin_tx(uint8_t ENABLE_or_DISABLE);
void mb_pin_rx(uint8_t ENABLE_or_DISABLE);
void mb_init(uint8_t MB_MODE,UART_HandleTypeDef *huart,TIM_HandleTypeDef *htim);
void mb_uart_init(UART_HandleTypeDef *huart);
void mb_uart_set(UART_HandleTypeDef *huart);
void mb_uart_buffer_reset(void);
void mb_tim_set(TIM_HandleTypeDef *htim);
void mb_tim_init(TIM_HandleTypeDef *htim);
uint16_t CRC16 (const uint8_t *nData, uint16_t wLength);
uint8_t mb_check_crc16(uint8_t address);
uint8_t mb_get_num_pakets(void);
uint8_t mb_get_size_pakets(uint8_t address);
/*************************************************************************/
uint8_t mb_rtu_request_read_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_request_read_discrete_inputs(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_request_read_holding_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_request_read_input_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_request_write_single_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data);
uint8_t mb_rtu_request_write_single_register(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data);
uint8_t mb_rtu_request_write_multiple_coils(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *DATA);
uint8_t mb_rtu_request_write_multiple_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *DATA);

uint8_t mb_rtu_response_read_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA);
uint8_t mb_rtu_response_read_discrete_inputs(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA);
uint8_t mb_rtu_response_read_holding_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA);
uint8_t mb_rtu_response_read_input_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint8_t len,uint8_t *DATA);
uint8_t mb_rtu_response_write_single_coil(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t CAddress,uint16_t Data);
uint8_t mb_rtu_response_write_single_register(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t RAddress,uint16_t Data);
uint8_t mb_rtu_response_write_multiple_coils(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
uint8_t mb_rtu_response_write_multiple_registers(UART_HandleTypeDef *huart,uint8_t SlaveAd,uint16_t StartAd,uint16_t Quantity);
/*************************************************************************/
