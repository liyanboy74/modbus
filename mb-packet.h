#ifndef _MB_PACKET_C
#define _MB_PACKET_C

#ifdef __cplusplus
extern "C" {
#endif

#include "mb.h"

#define MB_PACKET_Buffer_Size MB_DEFAULT_BUFFER_SIZE

#if (MB_MODE==MB_MODE_MASTER) || defined MB_DEBUG

mb_packet_s mb_packet_request_read_coil(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_discrete_inputs(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_holding_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_read_input_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_request_write_single_coil(uint8_t DeviceAddress,uint16_t CAddress,uint16_t Data);
mb_packet_s mb_packet_request_write_single_register(uint8_t DeviceAddress,uint16_t RAddress,uint16_t Data);
mb_packet_s mb_packet_request_write_multiple_coils(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_request_write_multiple_registers(uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);

mb_packet_s mb_packet_type_master_request_var(mb_function_e function,uint8_t DeviceAddress,uint16_t StartAd,uint16_t Quantity,uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_type_master_request_fix(mb_function_e function,uint8_t DeviceAddress,uint16_t w1,uint16_t w2);

#endif
#if (MB_MODE==MB_MODE_SLAVE) || defined MB_DEBUG

mb_packet_s mb_packet_response_read_coil(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_discrete_inputs(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_holding_registers(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_input_registers(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_exeption_status(void);
mb_packet_s mb_packet_response_report_server_id(void);
mb_packet_s mb_packet_response_write_single_coil(uint16_t CAddress,uint16_t Data);
mb_packet_s mb_packet_response_write_single_register(uint16_t RAddress,uint16_t Data);
mb_packet_s mb_packet_response_write_multiple_coils(uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_response_write_multiple_registers(uint16_t StartAd,uint16_t Quantity);
mb_packet_s mb_packet_response_read_write_multiple_registers(uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_response_read_device_identification(uint8_t o_id);
mb_packet_s mb_packet_response_mask_write_register(uint8_t *payload);
mb_packet_s mb_packet_error(mb_function_e Func,mb_error_e Exeption_Code);

mb_packet_s mb_packet_type_slave_responce_var(mb_function_e function,uint8_t len,uint8_t *Data);
mb_packet_s mb_packet_type_slave_responce_fix(mb_function_e function,uint16_t w1,uint16_t w2);

#endif

#ifdef __cplusplus
}
#endif

#endif
