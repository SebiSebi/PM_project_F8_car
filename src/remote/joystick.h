#ifndef JOYSTICK_H__
#define JOYSTICK_H__

#include <stdint.h>

void JS_init();

/* Returns a percentage 0 - 100%;
 * 0% => 0V
 * 100% => 5V
 */
int8_t JS_getX();
int8_t JS_getY();

#endif  /* JOYSTICK_H__ */
