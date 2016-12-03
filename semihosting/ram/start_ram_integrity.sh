#!/bin/bash

DATA_START=0x0007AED0
DATA_END=0x0007afcc
TIME_START=1MS
TIME_DURATION=50MS

QEMU=qemu-system-arm
BIN_FILE=ram_integrity
INJECTION_FILE=injection_ram_integrity.xml
INJECTION_CNT=8

DATA_TOTAL=$(echo "$((DATA_END - $DATA_START))")

#Generate injection configuration file
echo \
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<injection>" > $INJECTION_FILE

for i in `seq 1 $INJECTION_CNT`;
do
	#Generate injection memory cell position with alignment of 4 (32b)
	num=$RANDOM
	MEM_CELL=$(( num %= $DATA_TOTAL ))
	MEM_CELL=$(echo "$(($DATA_START + $MEM_CELL))")
	ALIGN=$(echo "$(($MEM_CELL % 4))")
	MEM_CELL=$(echo "$(($MEM_CELL - $ALIGN))")
	MEM_CELL=0x$(printf '%x\n' $MEM_CELL)

	#Generate bit-flip position for 32b memory cell
	num=$RANDOM
	BIT=$(( num %= 32 ))
	BIT=$(echo "$((2 ** $BIT))")
	BIT=0x$(printf '%x\n' $BIT)

	echo \
"	<fault>
		<id>$i</id>
		<component>RAM</component>
		<target>MEMORY CELL</target>
		<mode>BIT-FLIP</mode>
		<trigger>ACCESS</trigger>
		<type>TRANSIENT</type>
		<timer>$TIME_START</timer>
		<duration>$TIME_DURATION</duration>
		<params> 
			<address>$MEM_CELL</address>
			<mask>$BIT</mask>
		</params>
	</fault>" >> $INJECTION_FILE
done

echo "</injection>" >> $INJECTION_FILE

make
$QEMU -semihosting -kernel $BIN_FILE -fi $INJECTION_FILE

