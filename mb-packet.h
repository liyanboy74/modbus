#ifndef _MB_PACKET_C
#define _MB_PACKET_C

#include "mb.h"

mb_packet_s mb_packet_request_read_coil(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_discrete_inputs(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_holding_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_input_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_write_single_coil(uint8_t DeviceAddress,uint16_t CAddress,uint16_t Data);
mb_packet_s mb_packet_request_write_single_register(uint8_t DeviceAddress,uint16_t RAddress,uint16_t Data);
mb_packet_s mb_packet_request_write_multiple_coils(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_request_write_multiple_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);


mb_packet_s mb_packet_response_read_coil(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_discrete_inputs(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_holding_registers(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_input_registers(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_write_single_coil(uint16_t CAddress,uint16_t Data);
mb_packet_s mb_packet_response_write_single_register(uint16_t RAddress,uint16_t Data);
mb_packet_s mb_packet_response_write_multiple_coils(uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_response_write_multiple_registers(uint16_t StartAd,uint16_t Quantity);

mb_packet_s mb_packet_error(mb_functions_e Func,mb_error_e Exeption_Code);

#endif
