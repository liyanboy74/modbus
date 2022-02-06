#ifndef _MB_H_
#define _MB_H_

//ModBus Fanctions
typedef enum{
	MB_Read_Coils					= 0x01,
	MB_Read_Discrete_Inputs			= 0x02,
	MB_Read_Holding_Registers		= 0x03,
	MB_Read_Input_Registers			= 0x04,
	MB_Write_Single_Coil			= 0x05,
	MB_Write_Single_Register		= 0x06,
	MB_Write_Multiple_Coils			= 0x0f,
	MB_Write_Multiple_Registers		= 0x10,
}mb_functions_e;

#endif