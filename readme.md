# MODBUS Lib
Modbus RTU C library

### Guide 
Follow below steps:
- Select master or slave mode in [mb.h](https://github.com/liyanboy74/modbus/blob/170f567ade8fa8fdf8a1529b4a969c3b3621665f/mb.h#L15) .
- Use [mb_set_tx_handler()](https://github.com/liyanboy74/modbus/blob/170f567ade8fa8fdf8a1529b4a969c3b3621665f/mb.h#L85) for set callback for transmit data from MODBUS layer.
- Send Byte-by-Byte received data to [mb_rx_new_data(Byte)](https://github.com/liyanboy74/modbus/blob/170f567ade8fa8fdf8a1529b4a969c3b3621665f/mb.h#L87) for processing .
- In master mode, received packet don't process automatically! so the callback must be set by [mb_set_master_process_handler()](https://github.com/liyanboy74/modbus/blob/170f567ade8fa8fdf8a1529b4a969c3b3621665f/mb.h#L83) .
- See [example](test.c) .

**Note:** The library don't handle Timout error! the [mb_rx_timeout_handler()](https://github.com/liyanboy74/modbus/blob/aa819b01d9e52f9a2ba2f8f34170322398494f82/mb.h#L88) must be call for reset rx buffer index .

### File Description
**Common**
- `mb` : Common MODBUS init,functions and types
- `mb-link` : Prepare Packet for send, receive and process
- `mb-crc` : Cyclic redundancy check
- `mb-packet` : Make packets

**Slave**
- `mb-check` : Checking received data
- `mb-process` : Process received packets
- `mb-table` : Database

