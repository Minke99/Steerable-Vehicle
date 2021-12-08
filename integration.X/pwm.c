/* 
 * File:   PWM.c
 * Author: 490440906
 * Created on October 11, 2021
 * 
 * Description: 
 * - Configure the pins on the PIC18 corresponding to the power supply board.
 * - Configure CCP1 and CCP2 in PWM mode.
 * - Map user inputs from the hand controller 
 * - Control the motor speed and the direction of drive.
 */

#include "PWM.h"
#pragma code

// predefine variable 
#define central_position 125
#define speed_modulation 4
#define forward 1       //original 1
#define backward 0      //original 0

// set the speed and direction according to the user input
void AutoMapping(int joy_x, int joy_y){
    int x_in,y_in;
    int motor_left,motor_right;
    char motor_dir_left, motor_dir_right;
    
    x_in = joy_x; 
    y_in = joy_y;
    
    // determine the speed of the motor
    motor_left = speed_modulation*((x_in - central_position)+ (int) (0.3*(y_in - central_position)) );
    motor_right = speed_modulation*((x_in - central_position) - (int) (0.3*(y_in - central_position)));
    motor_right += (int) motor_right * 0.13;
    
    // determine the direction of the motor 
    if (motor_left >= 0){
        motor_dir_left = forward;
    }
    else{
        motor_dir_left = backward;
        motor_left *= -1;
    }
    
    if (motor_right >= 0){
        motor_dir_right = forward;
    }
    else{
        motor_dir_right = backward;
        motor_right *= -1;
    }
    
    // call on required function 
    SetDirection(motor_dir_left,motor_dir_right);
    SetSpeed(motor_left,motor_right);
    
    
}

// set the speed and direction according to the PID controller 
void mapping(int joy_x, int joy_y){
    int x_in,y_in;
    int motor_left,motor_right;
    char motor_dir_left, motor_dir_right;
       
    x_in = joy_x; 
    y_in = joy_y;
    
    // determine the speed of the motor
    motor_left = speed_modulation*((x_in - central_position)+ (y_in - central_position));
    motor_right = speed_modulation*((x_in - central_position) - (y_in - central_position));
    
    // determine the direction of the motor
    if (motor_left >= 0){
        motor_dir_left = forward;
    }
    else{
        motor_dir_left = backward;
        motor_left *= -1;
    }
    
    if (motor_right >= 0){
        motor_dir_right = forward;
    }
    else{
        motor_dir_right = backward;
        motor_right *= -1;
    }
    
    // call on required function 
    SetDirection(motor_dir_left,motor_dir_right);
    SetSpeed(motor_left,motor_right);
    
    
}

// function definition 
void SetSpeed(int speed_left ,int speed_right)
{
    // CCPR1L:CCP1CON<5:4> = PWM duty cycle/(Tosc*Timer2 prescale)
    //                     = (PWM period/4)/(Tosc*Timer2 prescale)
    //                     = (0.0004/4)/[(1/10M)*4] = 250 (25% duty cycle ratio)
    // CCPR1L:CCP1CON<5:4> = 500 (50% duty cycle ratio)
    int duty_cycle = speed_left;
    int duty_cycle1 = speed_right;
    int temp_duty_cycle = 0;
   
    // set duty cycle of both motors
    temp_duty_cycle = (duty_cycle & 0x03);
    CCP1CON |= (temp_duty_cycle << 4);
    CCPR1L = (duty_cycle >> 2);
    
    temp_duty_cycle = (duty_cycle1 & 0x03);
    CCP2CON |= (temp_duty_cycle << 4);
    CCPR2L = (duty_cycle1 >> 2);
}

void SetDirection(char direction_flag_left, char direction_flag_right)
{
    //left
    // driving backward
    if(direction_flag_left == 0)
    {
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;
    }
    // driving forward
    else if (direction_flag_left == 1)
    {    
        PORTDbits.RD1 = 1;
        PORTDbits.RD2 = 0;
    }
    
    //right
    //driving backward
    if(direction_flag_right == 0){
        PORTDbits.RD4 = 0;
        PORTDbits.RD5 = 1;
    }
    //driving forward
    else if (direction_flag_right == 1)
    {
        PORTDbits.RD4 = 1;
        PORTDbits.RD5 = 0;        
    }
}

void EnableMotor()
{
    PORTDbits.RD0 = 1;  // enable motor 1
    PORTDbits.RD3 = 1;  // enable motor 2
}

void DisableMotor()
{
    PORTDbits.RD0 = 0;  // disable motor 1
    PORTDbits.RD3 = 0;  // disable motor 2
}

void MotorSetup(int period){
    
    // set the PWM period by writing to PR2
    // assume PWM frequency = 2.5kHz, PWM period = 1/2.5k = 0.0004s
    // PR2 = PWM period/(Tosc*4*Timer2 prescale)-1
    //     = 0.0004/[(1/10M)*4*4]-1 = 249
    PR2 = period;
    
    
    // set PORTD<5:0> as output
    TRISDbits.RD0 = 0;    // RD0 --> Motor 1 Enable
    TRISDbits.RD1 = 0;    // RD1 --> Motor 1 Input A
    TRISDbits.RD2 = 0;    // RD2 --> Motor 1 Input B
    
    TRISDbits.RD3 = 0;    // RD3 --> Motor 2 Enable
    TRISDbits.RD4 = 0;    // RD4 --> Motor 2 Input A
    TRISDbits.RD5 = 0;    // RD5 --> Motor 2 Input B
        
    // set PORTC<2:1> as output 
    TRISBbits.RB3 = 0;    // RB3/CCP2 --> Motor 2 PWM input 
    TRISCbits.RC2 = 0;    // RC2/CCP1 --> Motor 1 PWM input 
    
    // set the TMR2 prescale value and enable Timer2 by writing to T2CON
    T2CONbits.TMR2ON = 1;   // enable Timer2
    T2CONbits.T2CKPS1 = 0;  // TMR2 prescale value 4
    T2CONbits.T2CKPS0 = 1;  
    
    // configure the CCP1 module for PWM operation
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    
    // configure the CCP2 module for PWM operation
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
}
