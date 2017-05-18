all: lab1.hex master.hex

lab1.elf: lab1.c lcd.c usart.c HC_SR04.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

lab1.hex: lab1.elf
	avr-objcopy -j .text -j .data -O ihex lab1.elf lab1.hex
	avr-size lab1.elf

master.elf: master.c usart.c lcd.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

master.hex: master.elf
	avr-objcopy -j .text -j .data -O ihex master.elf master.hex
	avr-size master.elf

clean:
	rm -rf lab1.elf lab1.hex master.hex master.elf

.PHONY: all clean

