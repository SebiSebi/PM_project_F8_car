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

void process_distance_command()
{
	char dist[64];
	sprintf(dist, "%d", (int)HC_SR04_get_distance_auto()); 
	strcat(dist, " cm");
	BT_put(dist);		
}

void process_car_command()
{
	char msg[64];

	BT_get(msg, 64);
	int speed = atoi(msg);	

	BT_get(msg, 64);
	int steer = atoi(msg);

	PORTD ^= (1 << PD2);

	set_car_configuration(speed, steer);
}

int main()
{
	sei();
	DDRB |= (1 << PB0); /* buzzer */
	PORTB &= ~(1 << PB0);

	USART0_init();
	BT_init_slave();

	/* Enable receive interrupts on USART0 */
	// UCSR0B |= (1 << RXCIE0);

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
	}

	return 0;
}

