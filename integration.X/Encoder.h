/* 
 * File:   Encoder.h
 * Author:  Qingyi Wang
 * Created on October 22, 2021
 * 
 * Description:
 * - Configure external interrupt to handle pulse counting.
 * - Configure timer 1 overflow to measure the motor speed every 0.1s.
 * - Count the number pulses generated by the encoder.
 * - Calculated the speed of motor by pulse counting.
 */

#ifndef ENCODER_H
#define	ENCODER_H

// enable PORTB<2:0> and PORTB<4> for external interrupt
void Enable_PortB(void);

// enable Timer1 and preload the sampling time interval into TMR1
// If the TMR1 overflows, the Timer1 interrupt will be triggered,
// while four encoder counting variables should be reset.
void Timer1_Setup(void);

// configure PORTB<2:0> and PORTB<4> for external interrupt
void Encoder_Interrupt_Setup(void);

void Enable_Interrupt(void);
void Disable_Interrupt(void);

// count the number of pulses per channel
void Count_Pulse_Per_Channel(int Encoder_Channel);

// calculate the speed of each motor
double Get_RPM(int Motor_Number);

// reset four counter variables
void Reset_Counter(void);

// handle the TMR1 overflows
void TMR1_Overflow(void);
    
#endif
