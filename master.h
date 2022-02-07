#include <stdint.h>
#include "mb.h"

mb_error_e mb_func_supported(uint8_t func);
mb_error_e mb_check_quantity(uint16_t Quantity);
mb_error_e mb_check_coils_address(uint16_t StartAd,uint16_t Quantity);
