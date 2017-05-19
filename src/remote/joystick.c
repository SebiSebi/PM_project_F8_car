#include "joystick.h"

#include <avr/io.h>

void JS_init()
{
	ADMUX = (1 << REFS0); /* Compare with AVCC. */
	
    	ADCSRA = (1 << ADEN) | (5 << ADPS0); /* Enable ADC */
}

/* Returns voltage */
static double sample_channel(uint8_t channel)
{
	/* Select channel */
	ADMUX &= 0b11100000;
	ADMUX |= (channel << MUX0);

	/* Start conversion */
	ADCSRA |= (1 << ADSC);
    	while(ADCSRA & (1 << ADSC));

	return ADC * 5.0 / 1024.;
}

int8_t JS_getX()
{
	return ((2.52 - sample_channel(0)) / 2.48 * 100);
}

int8_t JS_getY()
{
	return ((2.50 - sample_channel(1)) / 2.48 * 100);
}

