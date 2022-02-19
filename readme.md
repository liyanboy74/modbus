# MODBUS
Modbus RTU C library for embedded systems.

### Guide 
Follow below steps:
- Select master or slave mode in [mb.h](https://github.com/liyanboy74/modbus/blob/a743aaea0edec93026f17e09d0ca442f9a532cf7/mb.h#L16) or in Compiler Defines.
- Use [mb_set_tx_handler()](https://github.com/liyanboy74/modbus/blob/a743aaea0edec93026f17e09d0ca442f9a532cf7/mb.h#L88) for set callback for transmit data from MODBUS layer.
- Send Byte-by-Byte received data to [mb_rx_new_data()](https://github.com/liyanboy74/modbus/blob/a743aaea0edec93026f17e09d0ca442f9a532cf7/mb.h#L90) for processing .
- In master mode, received packet don't process automatically! so the callback must be set by [mb_set_master_process_handler()](https://github.com/liyanboy74/modbus/blob/a743aaea0edec93026f17e09d0ca442f9a532cf7/mb.h#L86) .
- See [test.c](test.c) .
- It's recommended to use [FIFO](https://github.com/liyanboy74/fifo) on input data befor sending to MODBUS .

**Note:** The library don't handle Rx Timout error! the [mb_rx_timeout_handler()](https://github.com/liyanboy74/modbus/blob/aa819b01d9e52f9a2ba2f8f34170322398494f82/mb.h#L88) must be call for reset rx buffer index .

### Examples
- [STM32 Slave MODBUS Example](https://github.com/ioelectro/modbus-stm32-slave-example.git)
- [AVR MODBUS Network Example](https://github.com/ioelectro/modbus-avr-example.git)

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

