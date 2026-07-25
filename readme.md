# MODBUS

A lightweight MODBUS C library designed specifically for embedded systems.

The library completely separates the MODBUS protocol core from the transport layer, allowing the exact same core implementation to run seamlessly over different communication interfaces.

### Supported Transport Layers
*   **MODBUS RTU**
*   **MODBUS TCP**

---

## Features

*   **Dual Mode:** Supports both MODBUS Master and Slave.
*   **Transport Flexibility:** Full MODBUS RTU and TCP support.
*   **Embedded Friendly:** Zero dynamic memory allocation (`malloc`).
*   **Highly Portable:** Compliant with the C99 standard.
*   **Customizable:** Configurable function code support to optimize footprint size.

---

## Quick Start Guide

### Select Operating Mode
You can configure the operating mode as either **Master** or **Slave** inside `mb.h` or directly via **compiler definitions**.

**Option 1: In `mb.h` file**
```c
#define MB_MODE MB_MODE_SLAVE
```

**Option 2: Via GCC compiler flag**
```bash
gcc ... -DMB_MODE=MB_MODE_MASTER
```

---

## Transport Layer

To minimize code size, only one transport layer should be compiled at a time. The MODBUS core remains entirely agnostic of the underlying hardware interface.

*   `mb-rtu.c` – Handles serial communication.
*   `mb-tcp.c` – Handles network communication.

### Receiving Data

#### MODBUS RTU
Received UART bytes must be passed to the library byte-by-byte:
```c
mb_rx_new_data(Byte);
```
Since the library does not generate timeout events internally, your application must call the timeout handler whenever an RTU frame timeout occurs ( for reset rx buffer index if received packet was brok! ) :
```c
mb_rx_timeout_handler();
```

#### MODBUS TCP
Received TCP frames must be passed as a complete data buffer:
```c
mb_tcp_receive(buffer, length);
```
*Note: TCP does not require CRC checking or MODBUS timeout handling, as packet integrity is automatically managed by the TCP/IP stack.*



## Data Transmission

### 1. Register the Transmit Callback
You need to register the transport layer's transmit function so the core can send packets out:
```c
mb_set_tx_handler(tx_function);
```

**Example Implementation:**
```c
void send_data(uint8_t *Data, uint16_t Len)
{
    // Implementation: Send data through UART or TCP socket
}
```

### 2. Master Mode Processing
In Master mode, incoming packets are not processed automatically. You must register a response handler callback:
```c
mb_set_master_process_handler(callback);
```

**Example Implementation:**
```c
void master_process(mb_packet_s Packet)
{
    // Implementation: Process the received response from the slave
}
```

---

## File Architecture

| Category | File | Description |
| :--- | :--- | :--- |
| **Common** | `mb.h` | Common configurations, definitions, and core data types. |
| | `mb.c` | Core MODBUS protocol implementation. |
| | `mb-packet.c` | Packet creation, formatting, and parsing logic. |
| | `mb-crc.c` | CRC-16 calculation utilities. |
| **Transport** | `mb-rtu.c` | MODBUS RTU transport layer implementation. |
| | `mb-tcp.c` | MODBUS TCP transport layer implementation. |
| **Slave Core** | `mb-check.c` | Validation and verification of received requests. |
| | `mb-process.c` | Execution and processing of MODBUS function codes. |
| | `mb-table.c` | Slave register database maps. |

---

## Supported Function Codes

| Code | Function Name |
| :---: | :--- |
| **0x01** | Read Coils |
| **0x02** | Read Discrete Inputs |
| **0x03** | Read Holding Registers |
| **0x04** | Read Input Registers |
| **0x05** | Write Single Coil |
| **0x06** | Write Single Register |
| **0x07** | Read Exception Status |
| **0x0F** | Write Multiple Coils |
| **0x10** | Write Multiple Registers |
| **0x17** | Read/Write Multiple Registers |
| **0x2B** | Encapsulated Interface Transport |

---

## Testing & Compilation

To build all available test cases, navigate to the test directory and run make:
```bash
cd test
make
```

### Generated Binaries
*   `slave` – MODBUS RTU Slave test application.
*   `master` – MODBUS RTU Master test application.
*   `server` – MODBUS TCP Slave test application.
*   `client` – MODBUS TCP Master test application.

### Running RTU Tests
```bash
# Run Slave
make slave

# Run Master
make master
```

### Running TCP Tests
*Default TCP Port used: **1502***

```bash
# Start the TCP Server
make server
./server

# In another terminal, start the TCP Client
make client
./client
```

---

## External Tools & Examples

### Tools
*   [Radzio Modbus Master Simulator](https://en.radzio.dxp.pl/modbus-master-simulator/)

### Reference Examples
*   [STM32 MODBUS Slave Example](https://github.com/ioelectro/modbus-stm32-slave-example.git)
*   [STM8 MODBUS Slave Example](https://github.com/ioelectro/modbus-stm8-slave-example)
*   [AVR MODBUS Network Example](https://github.com/ioelectro/modbus-avr-example.git)
