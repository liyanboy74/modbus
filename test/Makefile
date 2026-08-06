TARGET = mb

CC = gcc

DEFINES =

WARN = -std=c99 \
	-Wall -Wextra -Wpedantic \
	-Wconversion -Wsign-conversion \
	-Wshadow -Wformat=2 \
	-Wundef -Wstrict-prototypes \
	-Wmissing-prototypes

INC = -I..

SRC_SLAVE = \
../mb-check.c \
../mb-crc.c \
../mb-packet.c \
../mb-process.c \
../mb-table.c \
../mb.c

SRC_MASTER = \
../mb-crc.c \
../mb-packet.c \
../mb.c

SRC_RTU = \
../mb-rtu.c

SRC_TCP = \
../mb-tcp.c

default: test

slave:
	$(CC) $(WARN) $(INC) -DMB_MODE=MB_MODE_SLAVE  test-rtu-slave.c \
	$(SRC_SLAVE) $(SRC_RTU) $(DEFINES) -o slave
	./slave

master:
	$(CC) $(WARN) $(INC) -DMB_MODE=MB_MODE_MASTER  test-rtu-master.c \
	$(SRC_MASTER) $(SRC_RTU) $(DEFINES) -o master
	./master

server:
	$(CC) $(WARN) -DMB_MODE=MB_MODE_SLAVE test-tcp-server.c \
	$(SRC_SLAVE) $(SRC_TCP) $(INC) $(DEFINES) -o server

client:
	$(CC) $(WARN) -DMB_MODE=MB_MODE_MASTER test-tcp-client.c \
	$(SRC_MASTER) $(SRC_TCP) $(INC) $(DEFINES) -o client

server-rtu:
	$(CC) $(WARN) -DMB_MODE=MB_MODE_SLAVE test-rtu-over-tcp-server.c \
	$(SRC_SLAVE) $(SRC_RTU) $(INC) $(DEFINES) -o server-rtu

test: slave master server client server-rtu

clean:
	rm -rf *.o *.exe slave master server client server-rtu
