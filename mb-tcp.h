#ifndef _MB_TCP_H_
#define _MB_TCP_H_

#include <stdint.h>
#include "mb.h"

#define MB_TCP_ADU_MAX_SIZE 260
#define MB_TCP_Tx_Buffer_Size   MB_TCP_ADU_MAX_SIZE

typedef enum
{
    MB_TCP_OK,
    MB_TCP_ERROR_LENGTH,
    MB_TCP_ERROR_PROTOCOL_ID,
    MB_TCP_ERROR_FUNC
} mb_tcp_error_e;

#ifdef __cplusplus
extern "C" {
#endif

void mb_tcp_receive(uint8_t *Data, uint16_t Len);
void mb_tcp_send(uint8_t *Data, uint16_t Len);

// TCP Transport
void mb_tx_packet_handler(mb_packet_s Packet);

#ifdef __cplusplus
}
#endif

#endif