#include "../shared/usart.h"
#include "../shared/lcd.h"
#include "../shared/bluetooth.h"
#include "joystick.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

int main()
{
    	LCD_init();
	USART0_init();
	BT_init_master();
	
	JS_init();
	while(1) {
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

		_delay_ms(150);	
	}

	while(1);

	return 0;
}

