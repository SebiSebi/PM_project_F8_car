#ifndef HC_SR04_H__
#define HC_SR04_H__

#define HC_SR04_NUM_ITERATIONS 		1
#define MINIMUM_SAFE_DISTANCE_STATIC	10.0f  /* centimeters */
#define MINIMUM_SAFE_DISTANCE_MOVING	50.0f  /* centimeters */

/* This must be called before calling any other function. */
void HC_SR04_init();

/* Single sample of the distance */
float HC_SR04_get_distance();

/**
 * Call this to sample every TODO ms. 
 * After you call this function, @HC_SR04_get_distance()
 * MUST NOT be called, otherwise it will result in faulty distances.
 * Use HC_SR04_get_distance_auto() to receive the distance
 */
void HC_SR04_set_auto_mode();
float HC_SR04_get_distance_auto();

#endif  /* HC_SR04_H__ */
