#ifndef _MB_CHEAK_H_
#define _MB_CHEAK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mb.h"

#if(MB_MODE==MB_MODE_SLAVE)

// Enable or Disable Support Modbus Function for Slave Mode
#define MB_ENABLE_FUNC_Read_Coils               1
#define MB_ENABLE_FUNC_Read_Discrete_Inputs     1
#define MB_ENABLE_FUNC_Read_Holding_Registers   1
#define MB_ENABLE_FUNC_Read_Input_Registers     1
#define MB_ENABLE_FUNC_Write_Single_Coil        1
#define MB_ENABLE_FUNC_Write_Single_Register    1
#define MB_ENABLE_FUNC_Write_Multiple_Coils     1
#define MB_ENABLE_FUNC_Write_Multiple_Registers 1


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