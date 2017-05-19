#include "HC_SR04.h"

#include <avr/interrupt.h>
#include <util/delay.h>

static volatile uint8_t flag = 0;
static volatile uint8_t count = 0;
static volatile uint8_t saved_tcnt0 = 0;
static volatile uint8_t saved_count = 0;
static volatile float saved_distance = 0.0f;
static volatile uint8_t timer2_count = 0;

ISR(TIMER0_OVF_vect, ISR_NOBLOCK) // Other interrupts aren't blocked during this routine
{
	count++;
}

ISR(PCINT0_vect, ISR_NOBLOCK) // Other interrupts are blocked during this routine
{
	if ((PINA & (1 << PA1)) != 0) {
		/* Echo rising edge */
		TCNT0 = 0;
		count = 0;
	}
	else {
		/* Echo falling edge */
		saved_tcnt0 = TCNT0;
		saved_count = count;
		flag = 1;
	}
}

void HC_SR04_init()
{
	TCCR0A = 0;
	TCCR0B = 0;
	TIMSK0 = 0;

	sei(); // Enable global interrupts
	TCCR0B |= (1 << CS02); // Prescaler = 256 => T = 16us
	TIMSK0 |= (1 << TOIE0); // Enable Overflow interrupt

	/* Configure pins for trigger and echo */
	DDRA |= (1 << PA0); // Trigger
	DDRA &= ~(1 << PA1); // Echo

	PORTA &= ~(1 << PA0);

	/* Enable pin change interrupt on echo pin */
	PCICR |= (1 << PCIE0);
	PCMSK0 = 0;
	PCMSK0 |= (1 << PCINT1);
}

void HC_SR04_set_auto_mode()
{
	/* Use Timer2 to generate software interrupts */
	TCCR2A = 0;
	TCCR2B = 0;
	TIMSK2 = 0;
	timer2_count = 0;
	sei();

	/* CTC with TOP at OCRA */	
	TCCR2A |= (1 << WGM21);
	
	/* Prescaler 1024 */
	TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);

	OCR2A = 255;
	TIMSK2 |= (1 << OCIE2A); /* Enable interrupt on match OCR2A */
}

ISR(TIMER2_COMPA_vect, ISR_NOBLOCK)
{
	timer2_count++;
	if (timer2_count == 12) {
		TIMSK2 &= ~(1 << OCIE2A); /* Temporary disable this interrupt */
	
		saved_distance = HC_SR04_get_distance();
		if (saved_distance <= 15.0f)
			PORTB |= (1 << PB0);
		else PORTB &= ~(1 << PB0);

		timer2_count = 0;
		TCNT2 = 0; /* Reset counter */
		TIMSK2 |= (1 << OCIE2A); /* Restore interrupt */
	}	
}

float HC_SR04_get_distance_auto()
{
	return saved_distance;
}

float HC_SR04_get_distance()
{
	double sum = 0;
	for (int i = 1; i <= HC_SR04_NUM_ITERATIONS; i++) {
		flag = 0;
		
		/* Reset Trigger signal for a clean rise */
		PORTA &= ~(1 << PA0);
		_delay_us(5);

		/* Send Trigger signal for > 10us */
		PORTA |= (1 << PA0);
		_delay_us(11);
		PORTA &= ~(1 << PA0);
		
		/* Wait for the Echo */
		while(flag == 0 && count <= 5);
	
		sum = sum + (((int)saved_tcnt0 + 255 * saved_count) * 16.0) * 0.017;
	
		if (i < HC_SR04_NUM_ITERATIONS)
			_delay_ms(75);
	}

	return sum / HC_SR04_NUM_ITERATIONS;
}

