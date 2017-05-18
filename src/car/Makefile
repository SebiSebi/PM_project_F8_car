SHARED=../shared/usart.c ../shared/bluetooth.c

all: car.hex

car.elf: car.c $(SHARED)
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

car.hex: car.elf
	avr-objcopy -j .text -j .data -O ihex car.elf car.hex
	avr-size car.elf

clean:
	rm -rf car.elf car.hex

.PHONY: all clean

