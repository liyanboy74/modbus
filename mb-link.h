#ifndef _MB_LINK_H_
#define _MB_LINK_H_

#include <stdint.h>

#define MB_LINK_Rx_Buffer_Size     64 //MAx 256
#define MB_LINK_Rx_MDBL  (MB_LINK_Rx_Buffer_Size-9)

typedef enum{
    MB_LINK_OK,
    MB_LINK_ERROR_Address,
    MB_LINK_ERROR_Data_Size,
    MB_LINK_ERROR_CRC,
    MB_LINK_ERROR_FUNC,

}mb_link_error_e;

void mb_link_check_new_data(uint8_t Byte);
void mb_link_reset_rx_buffer(void);

#endif
