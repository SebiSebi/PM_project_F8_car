#ifndef MOTOR_H__
#define MOTOR_H__

void DC_motor_init();

/**
 * @speed between -100 and 100.
 * @steer between -100 and 100.
 */
void set_car_configuration(int speed, int steer);
void set_car_binary_configuration(int speed, int steer);
int get_car_speed(void);

void set_motor_forward();
void set_motor_reverse();
void set_left_motors_forward();
void set_right_motors_forward();
void set_left_motors_reverse();
void set_right_motors_reverse();

void stop_motors();
void stop_left_motors();
void stop_right_motors();

#endif  /* MOTOR_H__ */
