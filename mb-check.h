#include "mb.h"

mb_error_e mb_check_func(uint8_t func);
mb_error_e mb_check_quantity(uint16_t Quantity);
mb_error_e mb_check_table_bit_address(uint16_t StartAd,uint16_t Quantity);
mb_error_e mb_check_table_address(uint16_t StartAd,uint16_t Quantity);
