TARGET = mb

CC = gcc

DEFINES = -DMB_DEBUG
WARN = -Wall

SRC = *.c

default:
	$(CC) $(WARN) $(SRC) $(DEFINES) -o $(TARGET)

clean:
	rm -rf *.o *.exe

test:
	$(CC) $(WARN) $(SRC) $(DEFINES) -DMB_MODE=MB_MODE_SLAVE -o $(TARGET)
	$(TARGET)
	$(CC) $(WARN) $(SRC) $(DEFINES) -DMB_MODE=MB_MODE_MASTER -o $(TARGET)
	$(TARGET)

