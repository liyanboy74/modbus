#ifndef _MB_CRC_H_
#define _MB_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum{
	MB_CRC_OK=0,
	MB_CRC_ERROR=1,
}mb_crc_e;

uint16_t mb_crc16 (const uint8_t *nData, uint16_t wLength);

uint8_t  mb_crc_add(uint8_t *Buffer,uint8_t Len);
mb_crc_e mb_crc_check(uint8_t *Buffer,uint8_t Len);

#ifdef __cplusplus
}
#endif

#endif
