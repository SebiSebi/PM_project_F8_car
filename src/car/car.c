#include "../shared/usart.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "../shared/bluetooth.h"
#include "../shared/lcd.h"
#include "../shared/usart.h"

int main()
{
	USART0_init();
	BT_init_slave();

	while(1);
	while(1) {
		char c[64];
		BT_get(c, 64);

		LCD_writeInstr(LCD_INSTR_clearDisplay);
		LCD_printAt(0, c);
	}
		
	return 0;
}

