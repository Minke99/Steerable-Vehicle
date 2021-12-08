/* 
 * PID Controller:   PID control for distance / heading
 *                  of robots
 * File:   PID.h
 * Author: Jiashu Wu
 * Last updated on 19 Nov 2021, 3:03 PM
 */

#ifndef PID_H
#define	PID_H

// Setup Function for PID controller
// Input: Void
// Output: Void
void PID_Setup(void);

// PID controller for keeping heading of the wall and robot parallel
// Input: Measurement difference between 2 left IR
// Output: 0-250 representation for steering
int Gradient_PID(float);

// PID controller for keeping robot 10cm away from the wall
// Input: Measurement of left front sensor
// Output: 0-250 representation for steering
int Displacement_PID(int error);

#endif

