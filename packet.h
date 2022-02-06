#ifndef _PACKET_C
#define _PACKET_C

#include <stdint.h>

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
