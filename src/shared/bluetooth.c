#include "bluetooth.h"
#include "usart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/**
 * The handshake protocol:
 * 
 * 		--------             -------
 * 		|master|	     |slave|
 * 		--------	     -------
 *
 *              Magic ch ---------->  recv ch
 *
 *       	recv ch2 <----------  ch + 1
 *		
 *		ch2 + 1  ---------->  recv ch2  
 *       	
 */
void BT_init_master(void)
{
	/* Wait for connection to be established. */
	_delay_ms(BT_PAIR_DELAY * 1000);

	char msg[3];
	msg[0] = BT_MAGIC_CH;
	msg[1] = '\0';

	/* Send magic character */
	BT_put(msg);

	/* Await for ACK */
	msg[0] = 0;
	BT_get(msg, 3);

	if (msg[0] != BT_MAGIC_CH + 1 || strlen(msg) != 1) {
		while(1); /* Faulty transmission */
	}

	/* Send BT_MAGIC_CH + 2 */
	msg[0] = BT_MAGIC_CH + 2;
	BT_put(msg);
}

void BT_init_slave(void)
{
	char msg[3], magic_ch;

	/* Turn off OK LED */
	DDRD |= (1 << PD2);
	PORTD &= ~(1 << PD2);

	/* Await for the magic character */
	BT_get(msg, 3);
	if (strlen(msg) != 1) {
		while(1); /* Faulty transmission */
	}

	magic_ch = msg[0];

	/* Send first ACK */
	msg[0] = magic_ch + 1;
	BT_put(msg);

	/* Wait for the response */
	msg[0] = 0xFF;
	BT_get(msg, 3);

	if (msg[0] != magic_ch + 2 || strlen(msg) != 1) {
		while(1);
	}

	/* Handshake ok -> turn on OK LED */		
	PORTD |= (1 << PD2);
}

void BT_get(char * const buf, unsigned int LEN_MAX)
{
	int cr = 0, lf = 0;
	unsigned int len = 0;
	
	while ((cr == 0 || lf == 0) && len + 1 < LEN_MAX) {
		char x = USART0_receive();
		if (x == 0xd) /* ASCII CR value */
			cr = 1;
		else if (x == 0xa) /* ASCIi LF value */
			lf = 1;
		else buf[len++] = x;
	}

	buf[len] = '\0';
}

void BT_put(const char *buf)
{
	unsigned int len = strlen(buf), i;

	for (i = 0; i < len; i++)
		USART0_transmit(buf[i]);

	/* Send STOP sequence */
	USART0_transmit('\r');
	USART0_transmit('\n');
}
