#include "../shared/usart.h"
#include "../shared/lcd.h"
#include "../shared/bluetooth.h"
#include "joystick.h"
#include "mpu6050/mpu6050.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>  //include libm

#define COMMAND_PERIOD 		200 /* in milliseconds */
#define JOYSTICK 		0
#define ACCELEROMETER 		1

void remote_init(void)
{
	/* Set horn pin as input. */
	DDRB &= ~(1 << PB0);
	PORTB |= (1 << PB0); /* Enable pull up resistor. */

	/* Set switch between joystick and accelerometer pin as input */
	DDRD &= ~(1 << PD5);
	PORTD |= (1 << PD5);
}

/* Returns 1 if the user has pressed the switch mode button, 0 otherwise. */
int switch_mode(void)
{
	/* No need for debounce here because of @COMMAND_PERIOD delay */
	if ((PIND & (1 << PD5)) == 0)
		return 1;
	return 0;
}

/* Returns 1 it the user has pressed the horn button, 0 otherwise */
int horn_button_pushed(void)
{
	if ((PINB & (1 << PB0)) == 0) {
		/* Software managed debouncing */
		_delay_ms(30);
		if ((PINB & (1 << PB0)) == 0) 
			return 1;
	}
	return 0;
}

int main()
{
	remote_init();
    	LCD_init();
	USART0_init();
	BT_init_master();
	JS_init();
	mpu6050_init();
	_delay_ms(50); /* Wait for initialization to propagate. */

	int mode = JOYSTICK;
	LCD_clear();
	LCD_printAt(0, "Use joystick to");
	LCD_printAt(40, "control the car");
	_delay_ms(1000);

	/* Every @COMMAND_PERIOD milliseconds send a command to the F8_CAR */ 
	while(1) {
		if (switch_mode()) {
			mode = 1 - mode;
			
			/* Reset car speed and steer. */
			BT_put("CAR");
			BT_put("0");  /* No speed. */
			BT_put("0");  /* No steer. */

			/* Display info on LCD. */
			LCD_clear();
			if (mode == JOYSTICK)
				LCD_printAt(0, "Joystick enabled");
			else
				LCD_printAt(0, "Rotate RC");
					
			_delay_ms(1000);
		}

		if (horn_button_pushed()) {
			/* Send horn command to the car */
			BT_put("HORN");
			BT_put("ON");
			_delay_ms(10);
			
			/* Reset car speed and steer. */
			BT_put("CAR");
			BT_put("0");  /* No speed. */
			BT_put("0");  /* No steer. */

			/* Wait for the button to be released. */
			while(horn_button_pushed());

			/* Turn off horn at car. */
			BT_put("HORN");
			BT_put("OFF");
		}

		/*
		char x[10];
		char y[10];
		x[0] = '\0'; y[0] = '\0';
		
		sprintf(x, "%d", JS_getX());
		sprintf(y, "%d", JS_getY());

		LCD_writeInstr(LCD_INSTR_clearDisplay);
		LCD_printAt(0, x);
		LCD_printAt(40, y);

		BT_put("CAR");
		BT_put(x);
		BT_put(y);
		*/

		_delay_ms(COMMAND_PERIOD);	
	}

	while(1);

	return 0;
}

