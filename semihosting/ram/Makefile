ANGEL_PATH=../angel/
ANGEL_LIB=$(ANGEL_PATH)/angel.a
CC=arm-none-eabi-gcc
CFLAGS=-std=gnu99 -Wall -I$(ANGEL_PATH)
LDFLAGS=--specs=nosys.specs
TARGET=ram_integrity

$(TARGET): ram_integrity.o hvd.o $(ANGEL_LIB)
	$(CC) $(LDFLAGS) ram_integrity.o hvd.o $(ANGEL_LIB) -o $(TARGET)
ram_integrity.o: ram_integrity.c hvd.h
	$(CC) $(CFLAGS) ram_integrity.c -c -o ram_integrity.o
hvd.o: hvd.c hvd.h
	$(CC) $(CFLAGS) hvd.c -c -o hvd.o

clean:
	rm -f *.o $(TARGET)

