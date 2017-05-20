SHARED=../shared/lcd.c ../shared/usart.c ../shared/bluetooth.c
ACCELEROMETER=mpu6050/mpu6050.c mpu6050/mpu6050dmp6.c i2chw/twimastertimeout.c

all: remote.hex

remote.elf: remote.c $(SHARED) $(ACCELEROMETER) joystick.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

remote.hex: remote.elf
	avr-objcopy -j .text -j .data -O ihex remote.elf remote.hex
	avr-size remote.elf

clean:
	rm -rf remote.elf remote.hex

.PHONY: all clean

