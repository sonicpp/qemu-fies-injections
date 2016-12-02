CC=arm-none-eabi-gcc
CFLAGS=-std=gnu99 -Wall
TARGET=angel.a

$(TARGET): angelio.o angel.o
	ar rcs $@ angelio.o angel.o
angelio.o: angelio.c angelio.h angel.h
	$(CC) $(CFLAGS) angelio.c -c -o angelio.o
angel.o: angel.c angel.h
	$(CC) $(CFLAGS) angel.c -c -o angel.o

clean:
	rm -f *.o $(TARGET)

