#include <stdint.h>

typedef enum{
	MB_CRC_OK=0,
	MB_CRC_ERROR=1,
}mb_crc_e;

uint16_t mb_crc16 (const uint8_t *nData, uint16_t wLength);

uint8_t  mb_add_crc(uint8_t *Buffer,uint8_t Len);
mb_crc_e mb_check_crc(uint8_t *Buffer,uint8_t Len);


