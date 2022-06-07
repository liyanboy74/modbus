# MODBUS
Modbus RTU C library for embedded systems.

![MBLib-data-flow](https://user-images.githubusercontent.com/64005694/154830661-b1cc3740-ec50-44c9-9330-b34d01b1224c.svg)

### Requirements
**Memory**
<table>
    <tr>
        <td></td>
        <td>Flash</td>
        <td>RAM</td>
    </tr>
    <tr>
        <td><strong>Master</td>
        <td>2K</td>
        <td>64</td>
    </tr>
    <tr>
        <td><strong>Slave</td>
        <td>4K</td>
        <td>96 *</td>
    </tr>
</table>
* Without database size, default +68 Byte.

### Guide 

Follow below steps:
- Select master or slave mode in `mb.h` or in Compiler Defines.
- Send Byte-by-Byte received data to `mb_rx_new_data()` for processing .
- Use `mb_set_tx_handler()` for set callback for transmit data from MODBUS layer.
- In master mode, received packet don't process automatically! so the callback must be set by `mb_set_master_process_handler()` .

**Note:** The library don't handle Rx Timout error! the `mb_rx_timeout_handler()` must be call for reset rx buffer index .

### Examples
- [STM32 MODBUS Slave Example](https://github.com/ioelectro/modbus-stm32-slave-example.git)
- [STM8 MODBUS Slave Example](https://github.com/ioelectro/modbus-stm8-slave-example)
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

### Run Test.c
After installing `GCC` and `Make` , run below command:
```
make test
```
