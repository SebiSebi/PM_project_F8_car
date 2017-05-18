SHARED=../shared/lcd.c ../shared/usart.c ../shared/bluetooth.c

all: remote.hex

remote.elf: remote.c $(SHARED)
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

remote.hex: remote.elf
	avr-objcopy -j .text -j .data -O ihex remote.elf remote.hex
	avr-size remote.elf

clean:
	rm -rf remote.elf remote.hex

.PHONY: all clean

