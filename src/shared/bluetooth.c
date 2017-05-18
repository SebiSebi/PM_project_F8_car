#include "bluetooth.h"
#include "lcd.h"
#include "usart.h"

#include <util/delay.h>
#include <string.h>

static void send_command(const char *cmd);
static void print_response();

void BT_init_master(void)
{
}

void BT_init_slave(void)
{

}

void BT_list_config(void)
{
	send_command("AT");
	send_command("AT+NAME?");
	send_command("AT+PSWD?");
	send_command("AT+ROLE?");
	send_command("AT+CMODE?");
	send_command("AT+RNAME?98d3,31,fd4470");
}

void BT_get(char *buf, unsigned int LEN_MAX)
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

static void send_command(const char *cmd) {
	LCD_writeInstr(LCD_INSTR_clearDisplay);
	LCD_printAt(0, cmd);
	_delay_ms(1500);
	for (unsigned int i = 0; i < strlen(cmd); i++)
		USART0_transmit(cmd[i]);
	USART0_transmit('\r');
	USART0_transmit('\n');
	print_response();
	_delay_ms(1500);
}

static void print_response()
{
	char c[64];
	BT_get(c, 64);

	char *p = strchr(c, ':');
	if (p == NULL)
		p = c;
	else p++;

	LCD_writeInstr(LCD_INSTR_clearDisplay);
	LCD_printAt(0, p);
}

