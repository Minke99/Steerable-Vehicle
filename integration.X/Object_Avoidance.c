/* 
 * Object Avoidance:    avoid objects based on left two IR reading
 *                      move back -> turn right -> move forward -> turn left
 * File:   Object_Avoidance.c
 * Author: 490189050, 490160392
 * Created on 3 Oct 2021, 3:00 PM
 */

#include <p18f452.h>
#include "PWM.h"
#include "Object_Avoidance.h"
#include "IR_Reader.h"
#include <Delays.h>

// define absolute function
#define abs(x) ((x)<0?-x:x)

// receive external flag
extern char emergency_stop_flag;

// save total number of overflow
unsigned int overflow_count = 0;
char count_up_flag = 0; //1 count up, 0 count down

// set motor to turn right
void turn_right()
{
    SetDirection(1,0);      // turning right
    SetSpeed(125,125);      // duty cycle ration 25%
}

// set motor to turn left
void turn_left()
{
    SetDirection(0,1);      // turning left
    SetSpeed(125,125);      // duty cycle ration 25%
}

// set motor to move forward
void move_forward()
{
    SetDirection(1,1);      // driving forward
    SetSpeed(125,125);      // duty cycle ration 25%
}

// set motor to move backward
void move_backward()
{
    SetSpeed(125,125);
    SetDirection(0,0);
}

// stop the vehicle
void stop()
{
    SetSpeed(0,0);
    SetDirection(1,1);
}

// When meeting the object, turn right first. 
// Record how many times did the vehicle turn.
void record_right()
{
    float distance_1, distance_2;
    float requirement;
    float comparison;
    
    requirement = 0.5;
    distance_1 = Read(1);
    distance_2 = Read(2);
    comparison = distance_1 - distance_2;
    count_up_flag = 1;
    overflow_count = 0;
    INTCONbits.TMR0IE = 1;
    
    while( abs(comparison) > requirement || distance_1 >= 10.0 ){
        if (emergency_stop_flag == 1)
        {
            break;
        }
        turn_right();
        distance_1 = Read(1);
        distance_2 = Read(2); 
        comparison = distance_1 - distance_2;
       
    }
    INTCONbits.TMR0IE = 0;
    count_up_flag = 0;
    SetSpeed(0,0); 
    SetDirection(1,1);      // driving forward
    
}

// Move forward until the obstacle is not visible in any of the sensor
void move_forward_until_pass(void)
{
    float distance_2;
    char count=0;
       
    distance_2 = Read(1);    

    SetSpeed(125,125);
    SetDirection(1,1);
    while(count!=3){
        if (emergency_stop_flag == 1)
        {
            break;
        }
        
        distance_2 = Read(1);
        if  (distance_2 >= 11)
        {
            count++;
        }
        else
        {
            count=0;
        }
        
    }
    TMR1_Overflow(10);
    SetSpeed(0,0);
    SetDirection(1,1);      // driving forward
}

// Return left as the same times as previous turned.
void turn_back_left(void)
{
    count_up_flag = 0;
    //overflow_count++;
    //overflow_count++;
    INTCONbits.TMR0IE = 1;
    while (INTCONbits.TMR0IE == 1){
        if (emergency_stop_flag == 1)
        {
            break;
        }
        turn_left();
        
    }
    SetSpeed(0,0);
    SetDirection(1,1);
    count_up_flag = 1;
}

// make the object avoidance
void avoidance()
{
    
    move_backward();    // move back
    TMR1_Overflow(10);  // delay
    stop();             // stop

    record_right();     // turn right until does not detect the object
    
    move_forward_until_pass();  // move forward until there is no obstacle
    
    turn_back_left();     // turn left back to original direction
}

// count the total number of overflow
void Timer0_overflow_counter(void)
{
    INTCONbits.TMR0IF = 0;
    if (count_up_flag)
        
    {
        overflow_count++;
    }
    else
    {
        overflow_count--;
        if (overflow_count == 0)
        {
            INTCONbits.TMR0IE = 0;
        }
                
    }
}

// set up timer 1 for delay
void Timer1_Setup(void)
{
    T1CONbits.RD16 = 1;     // configure Timer1 as a 16-bit timer/counter
    //T1CONbits.TMR1ON = 1;   // enable Timer1
    T1CONbits.T1CKPS1 = 1;  // 1:8 prescale value
    T1CONbits.T1CKPS0 = 1;
    
    // expect the sampling period is 0.1s
    // TMR1<H:L> = Resolution - [(T*Fosc)/(4*Prescale)]
    //           = 65535 - [(1*10M)/(4*8)] = 34285 in decimal
    // TMR1<H:L> = 1000 0101 1110 1101 in binary
    TMR1H = 0b10000101;
    TMR1L = 0b11101101;
    
    // If the TMR1 overflows, the Timer1 interrupt will be triggered,
    // while four encoder counting variables should be reset.
}

// delay function (0.21 s per each call)
void TMR1_Overflow(unsigned char n)
{
    unsigned char i = 0;
    T1CONbits.TMR1ON = 1;   // enable Timer1
   
    while (i<n)
    {
        if (emergency_stop_flag == 1)
        {
            break;
        }
        while(PIR1bits.TMR1IF == 0) {
            if (emergency_stop_flag == 1)
            {
                break;
            }
        }    // wait until the TMR1 overflow
        PIR1bits.TMR1IF = 0;    // clear TMR1 overflow interrupt flag bit   
        i++;
    }
    
    T1CONbits.TMR1ON = 0;   // disable Timer1
}