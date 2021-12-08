/* 
 * Object Avoidance:    avoid objects based on left two IR reading
 *                      move back -> turn right -> move forward -> turn left
 * File:   Object_Avoidance.h
 * Author: 490189050, 490160392
 * Created on 3 Oct 2021, 3:00 PM
 */

#ifndef OBJECT_AVOIDANCE_H
#define	OBJECT_AVOIDANCE_H

// set motor to turn right
void turn_right(void);

// set motor to turn left
void turn_left(void);

// set motor to move forward
void move_forward(void);

// set motor to move backward
void move_backward(void);

// stop the vehicle
void stop(void);

// record the time taken by right turn
// stop when parallel to the wall
void record_right(void);

// turn back left based on recorded time
void turn_back_left(void);

// make the object avoidance
void avoidance(void);

// Move forward until the obstacle is not visible in any of the sensor
void move_forward_until_pass(void);

// count the total number of overflow
void Timer0_overflow_counter(void);

// set up timer 1 for delay
void Timer1_Setup(void);

// delay function (0.21 s per each call)
void TMR1_Overflow(unsigned char n);
#endif	/* OBJECT_AVOIDANCE_H */

