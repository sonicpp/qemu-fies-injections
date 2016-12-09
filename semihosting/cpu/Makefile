ANGEL_PATH=../angel/
ANGEL_LIB=$(ANGEL_PATH)/angel.a
CC=arm-none-eabi-gcc
CFLAGS=-gstabs -g -std=gnu99 -Wall -I$(ANGEL_PATH)
LDFLAGS=--specs=nosys.specs
TARGETS=control_flow_1 control_flow_2

all: $(TARGETS)

control_flow_1: control_flow_1.o $(ANGEL_LIB)
	$(CC) $(LDFLAGS) control_flow_1.o $(ANGEL_LIB) -o control_flow_1
control_flow_1.o: control_flow_1.c
	$(CC) $(CFLAGS) control_flow_1.c -c -o control_flow_1.o
control_flow_2: control_flow_2.o $(ANGEL_LIB)
	$(CC) $(LDFLAGS) control_flow_2.o $(ANGEL_LIB) -o control_flow_2
control_flow_2.o: control_flow_2.c
	$(CC) $(CFLAGS) control_flow_2.c -c -o control_flow_2.o

clean:
	rm -f *.o $(TARGETS)

