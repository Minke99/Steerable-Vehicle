/* 
 * File:   PWM.h
 * Author: 490440906
 * Created on October 11, 2021
 * 
 * Description: 
 * - Configure the pins on the PIC18 corresponding to the power supply board.
 * - Configure CCP1 and CCP2 in PWM mode.
 * - Map user inputs from the hand controller 
 * - Control the motor speed and the direction of drive.
 */

#ifndef PWM_H
#define	PWM_H

#include "p18cxxx.h"

// function declaration

// set the speed and direction according to the user input
void mapping(int joy_x, int joy_y);

// set the speed and direction according to the PID controller 
void AutoMapping(int joy_x, int joy_y);

// configure CCP mode, pin assignment, period and duty cycle for PWM mode
void MotorSetup(int period);

// enable motor
void EnableMotor(void);

// disable motor
void DisableMotor(void);

// set the driving direction by passing the direction flag
void SetDirection(char direction_flag_left, char direction_flag_right);

// set the driving speed of two motors by changing the duty cycle
void SetSpeed(int speed_left ,int speed_right);

#endif