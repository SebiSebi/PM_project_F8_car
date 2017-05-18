#include "../shared/usart.h"
#include "../shared/lcd.h"
#include "../shared/bluetooth.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

int main()
{
    	LCD_init();
	USART0_init();
	BT_init_master();

	for (int i = 1; i <= 100000; i++) {
		char aux[10];
		sprintf(aux, "%d", i);
		BT_put(aux);
		_delay_ms(1000);
	}

	while(1);

	return 0;
}

