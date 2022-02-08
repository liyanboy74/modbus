#ifndef _MB_LINK_H_
#define _MB_LINK_H_

#include <stdint.h>

#define MB_LINK_Buffer_Size     64 //MAx 256
#define MB_LINK_Max_Data_Bytes_Len  (MB_LINK_Buffer_Size-9)

typedef enum{
    MB_LINK_OK,
    MB_LINK_ERROR_Address,
    MB_LINK_ERROR_Data_Size,
    MB_LINK_ERROR_CRC,
    MB_LINK_ERROR_FUNC,

}mb_link_error_e;

void mb_link_check_new_data(uint8_t Byte);
void mb_link_reset_buffer(void);

#endif
