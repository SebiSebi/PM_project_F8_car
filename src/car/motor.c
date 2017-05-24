#include "motor.h"
#include "HC_SR04.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define MIN_VALUE 1000
#define MAX_VALUE 4000
#define MAX_LIMIT 3900

volatile int8_t moving_forward = 0;
volatile static int car_speed = 0;
extern volatile uint8_t too_close_flag;  /* Defined in HC_SR04.c */ 

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
    ICR1 = MAX_VALUE; /* (100, 340 min), (10000, 2500) (MAX_VALUE0, 2 * 1850)*/
    OCR1A = MIN_VALUE;
    OCR1B = MIN_VALUE;
 
    /* Initialize motor direction. */
    DDRC =  0b00001111;
    PORTC = 0b00000000;

    /* The car is currently stopped. */
    moving_forward = 0;
}

void set_car_binary_configuration(int speed, int steer)
{
	car_speed = speed;
	if (speed > -10 && speed < 10) {
		moving_forward = 0;
		stop_motors();
		return ;
	}

	if (speed > 0 && too_close_flag == 1) {
		moving_forward = 1;
		stop_motors();
		return ;
	}

	if (speed > 0) {
		if (steer < -10) {
			set_left_motors_reverse();
			set_right_motors_forward();
		}
		else if (steer > 10) {
			set_left_motors_forward();
			set_right_motors_reverse();
		}
		else {
			set_left_motors_forward();
			set_right_motors_forward();
		}
		moving_forward = 1;
	}
	else {
		moving_forward = 0;
		if (steer < -10) {
			set_left_motors_forward();
			set_right_motors_reverse();
		}
		else if (steer > 10) {
			set_left_motors_reverse();
			set_right_motors_forward();
		}
		else {
			set_left_motors_reverse();
			set_right_motors_reverse();
		}

	}
}

void set_car_configuration(int speed, int steer)
{
	if (speed < -100) speed = -100;
	if (speed > 100) speed = 100;

	if (steer < -100) steer = -100;
	if (steer > 100) steer = 100;

	/* Save car speed. */
	car_speed = speed;

	cli();
	if (speed > -10 && speed < 10) {
		moving_forward = 0;
		stop_motors();
		sei();
		return ;
	}

	if (speed < 0) {
		moving_forward = 0;
		speed = -speed;
		set_motor_reverse();
	}
	else {
		moving_forward = 1;
		if (too_close_flag == 1) {
			stop_motors();
			sei();
			return ;
		}
		else set_motor_forward();
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

	left_speed = MIN_VALUE + (MAX_VALUE - MIN_VALUE) / 100 * left_speed;
	if (left_speed >= MAX_LIMIT)
		left_speed = MAX_LIMIT;

	right_speed = MIN_VALUE + (MAX_VALUE - MIN_VALUE) / 100 * right_speed;
	if (right_speed >= MAX_LIMIT)
		right_speed = MAX_LIMIT;

	OCR1A = left_speed;
	OCR1B = right_speed;
	sei();
}

/* From 0 to 100%. */
int get_car_speed(void)
{
	return car_speed;
}

void set_motor_reverse()
{
   	PORTC &= ~(0b00001111);
   	PORTC |= 0b00001010;
}

void set_motor_forward()
{
   	PORTC &= ~(0b00001111);
   	PORTC |= 0b00000101;
}

void set_left_motors_forward()
{
	PORTC &= ~(0b00000011);
    	PORTC |= 0b00000001;
}

void set_right_motors_forward()
{
	PORTC &= ~(0b00001100);
    	PORTC |= 0b00000100;
}

void set_left_motors_reverse()
{
	PORTC &= ~(0b00000011);
    	PORTC |= 0b00000010;
}

void set_right_motors_reverse()
{
	PORTC &= ~(0b00001100);
    	PORTC |= 0b00001000;
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
