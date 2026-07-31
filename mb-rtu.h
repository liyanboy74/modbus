#ifndef _MB_RTU_H_
#define _MB_RTU_H_

#include <stdint.h>

#include "mb.h"

// #define MB_RTU_DEBUG

#define MB_RTU_Rx_Buffer_Size     MB_DEFAULT_BUFFER_SIZE
#define MB_RTU_Tx_Buffer_Size     MB_DEFAULT_BUFFER_SIZE

#define MB_RTU_Rx_MDBL  (MB_RTU_Rx_Buffer_Size-9)

typedef enum{
    MB_RTU_OK,
    MB_RTU_ERROR_Address,
    MB_RTU_ERROR_Data_Size,
    MB_RTU_ERROR_CRC,
    MB_RTU_ERROR_FUNC
}mb_rtu_error_e;

typedef enum {
    MB_RTU_TYPE_VAR,
    MB_RTU_TYPE_FIX,
    MB_RTU_TYPE_ERROR,
    MB_RTU_TYPE_NONE
}mb_rtu_t_e;

typedef struct {
    uint8_t index;
    mb_rtu_t_e type;
}mb_rtu_clen_s;


#ifdef __cplusplus
extern "C" {
#endif

void            mb_rtu_check_new_data(uint8_t oneByte);
void            mb_rtu_reset_rx_buffer(void);

void            mb_rtu_send(uint8_t *Data,uint8_t Len);
void            mb_rtu_prepare_tx_data(mb_packet_s Packet);
void            mb_rtu_error_handler(mb_rtu_error_e err);
mb_packet_s     mb_rtu_rx_packet_split(uint8_t *Packet_Buffer,uint8_t Len);

#define         mb_rx_new_data(Byte)    mb_rtu_check_new_data(Byte)
#define         mb_rx_timeout_handler() mb_rtu_reset_rx_buffer()

// RTU Transport
void            mb_tx_packet_handler(mb_packet_s Packet);

#ifdef __cplusplus
}
#endif

#endif
