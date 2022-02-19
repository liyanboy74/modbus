# MODBUS
Modbus RTU C library for embedded systems.

![data-flow - small](https://user-images.githubusercontent.com/64005694/154813013-10ace85b-084d-4c21-b0ba-a9b96da182f0.jpg)
### Guide 

Follow below steps:
- Select master or slave mode in `mb.h` or in Compiler Defines.
- Use `mb_set_tx_handler()` for set callback for transmit data from MODBUS layer.
- Send Byte-by-Byte received data to `mb_rx_new_data()` for processing .
- In master mode, received packet don't process automatically! so the callback must be set by `mb_set_master_process_handler()` .
- It's recommended to use [FIFO](https://github.com/liyanboy74/fifo) on input data befor sending to MODBUS .

**Note:** The library don't handle Rx Timout error! the `mb_rx_timeout_handler()` must be call for reset rx buffer index .

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

