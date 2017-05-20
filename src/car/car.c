#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../shared/bluetooth.h"
#include "../shared/lcd.h"
#include "../shared/usart.h"
#include "HC_SR04.h"
#include "motor.h"

uint8_t horn_pressed = 0;

void process_distance_command(void)
{
	char dist[64];
	sprintf(dist, "%d", (int)HC_SR04_get_distance_auto()); 
	strcat(dist, " cm");
	BT_put(dist);		
}

void process_car_command(void)
{
	char msg[64];

	BT_get(msg, 64);
	int speed = atoi(msg);	

	BT_get(msg, 64);
	int steer = atoi(msg);

	PORTD ^= (1 << PD2);
	
	set_car_binary_configuration(speed, steer);
}

void process_horn_command(void)
{
	char msg[64];
	
	BT_get(msg, 64);
	if (strcmp(msg, "ON") == 0) {	
		horn_pressed = 1;
		PORTB |= (1 << PB0);
	}
	else {
		PORTB &= ~(1 << PB0);
		horn_pressed = 0;
	}
}

void car_init(void)
{
	sei(); /* Enable global interrupts */
	
	horn_pressed = 0;
	
	/* Set buzzer pin as output */
	DDRB |= (1 << PB0); /* buzzer */
	PORTB &= ~(1 << PB0);
}

int main()
{
	car_init();	
	USART0_init();
	BT_init_slave();

	/* Init distance module */
	HC_SR04_init();
	HC_SR04_set_auto_mode();

	/* Init motors */
	DC_motor_init();

	/* Process commands from the remote control */
	while(1) {
		char cmd[64];
		BT_get(cmd, 64);

		if (strcmp(cmd, "DIST") == 0)
			process_distance_command();
		else if (strcmp(cmd, "CAR") == 0)
			process_car_command();
		else if (strcmp(cmd, "HORN") == 0)
			process_horn_command();
	}

	return 0;
}

