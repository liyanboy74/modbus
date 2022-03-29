#ifndef _MB_CHEAK_H_
#define _MB_CHEAK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mb.h"

#if(MB_MODE==MB_MODE_SLAVE)

mb_error_e mb_check_func(uint8_t func);
mb_error_e mb_check_quantity(uint16_t Quantity);
mb_error_e mb_check_on_off(uint16_t Value);

mb_error_e mb_check_table_coils_address(uint16_t StartAd,uint16_t Quantity);
mb_error_e mb_check_table_discretes_input_address(uint16_t StartAd,uint16_t Quantity);
mb_error_e mb_check_table_input_registers_address(uint16_t StartAd,uint16_t Quantity);
mb_error_e mb_check_table_holding_registers_address(uint16_t StartAd,uint16_t Quantity);

mb_error_e mb_check_quantity_bit_n_byte(uint16_t Quantity,uint8_t N);
mb_error_e mb_check_quantity_reg_n_byte(uint16_t Quantity,uint8_t N);

#endif

#ifdef __cplusplus
}
#endif

#endif