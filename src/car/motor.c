#include "motor.h"

#include <avr/io.h>

void DC_motor_init()
{
    DDRD |= (1 << PIND5) | (1 << PIND4); /* Enable output driver. */
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << COM1B1);

    /* Set Fast PWM mode. */
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    /* Set the prescaler to 8. */
    TCCR1B |= (1 << CS11) | (1 << CS10);

    /* Set TOP (ICR1) to set the period. */
    ICR1 = 5000; /* (100, 340 min), (10000, 2500) (20000, 2 * 1850)*/
    OCR1A = 2500;
    OCR1B = 2500;

    /* Baterie plina (9.2V) la 50Hz PWM */
    /* 5000 -> 2.3V pe motoare -> nu pornesc */
    /* 7500 -> 3.3V pe motoare -> merg ok */
    /* 13500 -> 4.8V pe motor */

    /* Initialize motor direction */
    DDRC =  0b00001111;
    PORTC = 0b00000000;
}

void set_car_configuration(int speed, int steer)
{
	if (speed < -100) speed = -100;
	if (speed > 100) speed = 100;

	if (steer < -100) steer = -100;
	if (steer > 100) steer = 100;

	if (speed > -10 && speed < 10)
		stop_motors();
	else if (speed < 0) {
		speed = -speed;
		set_motor_reverse();
	}
	else {
		set_motor_forward();
	}

	int left_speed = speed;
	int right_speed = speed;

	if (steer < 0) {
		left_speed = left_speed - left_speed * (-steer) / 100;
	}
	else {
		right_speed = right_speed - right_speed * steer / 100;
	}

	if (left_speed < 10) 
		stop_left_motors();

	if (right_speed < 10)
		stop_right_motors();

	left_speed = 1700 + (5000 - 1700) / 100 * left_speed;
	if (left_speed >= 4850)
		left_speed = 4850;

	right_speed = 1700 + (5000 - 1700) / 100 * right_speed;
	if (right_speed >= 4850)
		right_speed = 4850;

	OCR1A = left_speed;
	OCR1B = right_speed;
}

void set_motor_reverse()
{
    PORTC &= ~(0b00001111);
    PORTC |= 0b00001010;
}

void set_motor_forward()
{
    PORTC &= ~(0b01111000);
    PORTC |= 0b00000101;
}

void stop_motors()
{
    PORTC &= ~(0b00001111);
}

void stop_left_motors()
{
	PORTC &= ~(0b00000011);
}

void stop_right_motors()
{
	PORTC &= ~(0b00001100);

}
