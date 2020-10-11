
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : flash01.gcc.thumb.sram.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex
	rm -f bintoh

#---------------------------------

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

flash01.gcc.thumb.o : flash01.c flash-bin.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c flash01.c -o flash01.gcc.thumb.o

flash01.gcc.thumb.sram.bin : sram.ld sram.o flash01.gcc.thumb.o
	$(ARMGNU)-ld -o flash01.gcc.thumb.sram.elf -T sram.ld sram.o flash01.gcc.thumb.o
	$(ARMGNU)-objdump -D flash01.gcc.thumb.sram.elf > flash01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy flash01.gcc.thumb.sram.elf flash01.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy flash01.gcc.thumb.sram.elf flash01.gcc.thumb.sram.bin -O binary

bintoh : bintoh.c
	gcc -O2 bintoh.c -o bintoh

flash-bin.h : bintoh flash-bin
	./bintoh flash-bin flash-bin.h
	
