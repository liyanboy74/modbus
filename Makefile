default:
	gcc -Wall mb.c mb-crc.c mb-packet.c mb-table.c mb-check.c mb-link.c test.c .\fifo\fifo.c -I.\fifo