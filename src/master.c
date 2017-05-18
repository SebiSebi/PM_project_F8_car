#include "usart.h"
#include "lcd.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define CAR_HC_NAME "F8_CAR"
#define CAR_HC_PASS = "7777"
#define CAR_HC_ADDRESS = "98:D3:31:FD:44:70"
#define CAR_HC_ADDRESS2 = "98d3:31:fd4470"

#define REMOTE_HC_NAME = "F8_Remote"
#define REMOTE_HC_PASS = "7777"

void get_message(char *c)
{
	int cr = 0, lf = 0, len = 0;
	
	while (cr == 0 || lf == 0) {
		char x = USART0_receive();
		if (x == 0xd)
			cr = 1;
		else if (x == 0xa)
			lf = 1;
		else c[len++] = x;
	}	
	c[len] = '\0';
}

void print_message()
{
	char c[64];
	get_message(c);

	LCD_writeInstr(LCD_INSTR_clearDisplay);
	LCD_printAt(0, c);
}

void send_command(const char *cmd) {
	LCD_writeInstr(LCD_INSTR_clearDisplay);
	LCD_printAt(0, cmd);
	_delay_ms(500);
	for (unsigned int i = 0; i < strlen(cmd); i++)
		USART0_transmit(cmd[i]);
	USART0_transmit('\r');
	USART0_transmit('\n');
	print_message();
	_delay_ms(500);
}

void init_bluetooth()
{
	//LCD_print("Starting...");
	send_command("AT");
	//send_command("AT+ORGL");
	//send_command("AT+ROLE=1");
	//send_command("AT+RESET");
	//send_command("AT+PSWD=7777");
	send_command("AT+INIT");
	//send_command("AT+RNAME?98d3,31,fd4470");
	//send_command("AT+PAIR=98d3,31,fd4470,10000");
	//send_command("AT+BIND=98d3,31,fd4470");
	send_command("AT+LINK=98d3,31,fd4470");
	//send_command("AT+ADDR");
	//send_command("AT+INIT");
	//send_command("AT+INQ");	
	/*	
	_delay_ms(1000);
	send_command("AT");	
	send_command("AT+NAME?");
	send_command("AT+ROLE?");
	*/
	/*
	send_command("AT+ROLE=1");
	send_command("AT+CMODE=0");
	send_command("AT+INIT");
	send_command("AT+INQ");
	*/
	// send_command("AT+ORGL");

}

void send_string(const char *cmd) {
	LCD_writeInstr(LCD_INSTR_clearDisplay);
	LCD_printAt(0, cmd);
	_delay_ms(500);
	for (unsigned int i = 0; i < strlen(cmd); i++)
		USART0_transmit(cmd[i]);
	USART0_transmit('\r');
	USART0_transmit('\n');
	_delay_ms(500);
}

void bluetooth()
{
	USART0_init();
	DDRB |= (1 << PB0);
	PORTB &= ~(1 << PB0);

	init_bluetooth();
	LCD_printAt(0, "DONE");
	_delay_ms(5000);	
	send_command("AT+RESET");
	
	for (int i = 0; i < 10000; i++) {
		char aux[10];
		sprintf(aux, "%d", i);
		send_string(aux);
		_delay_ms(500);
	}

	while (1) {
		// print_message();
		char c[64];
		get_message(c);

		if (strcmp(c, "on") == 0)
			PORTB |= (1 << PB0);
		else if (strcmp(c, "off") ==0)
			PORTB &= ~(1 << PB0);
		else {
			//LCD_writeInstr(LCD_INSTR_clearDisplay);
			//LCD_printAt(0, c);
		}
	}
}

int main()
{
    	LCD_init();

	while(1);

	bluetooth();
	
	return 0;
}

