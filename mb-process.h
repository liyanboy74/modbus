#ifndef _MB_PROCESS_H_
#define _MB_PROCESS_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "mb.h"

#if(MB_MODE==MB_MODE_SLAVE)

#define MB_PROCESS_Buffer_Size 64

void mb_slave_process_read_coils(mb_packet_s Packet);
void mb_slave_process_read_discrere_inputs(mb_packet_s Packet);
void mb_slave_process_read_holding_registers(mb_packet_s Packet);
void mb_slave_process_read_input_registers(mb_packet_s Packet);
void mb_slave_process_write_single_coil(mb_packet_s Packet);
void mb_slave_process_write_single_register(mb_packet_s Packet);
void mb_slave_process_write_multiple_coils(mb_packet_s Packet);
void mb_slave_process_write_multiple_register(mb_packet_s Packet);

#endif

#ifdef __cplusplus
}
#endif

#endif
