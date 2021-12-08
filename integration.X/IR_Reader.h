/* 
 * IR_reader: read analog signal from analog pin
 *            Convert analog data back to the distance in float
 *            Check whether there is an obstacle in the front
 * File:   IR_Reader.h
 * Author: 490189050, 490160392
 * Created on 3 Oct 2021, 3:00 PM
 */

#ifndef IRADC_H
#define	IRADC_H

// Threshold distance in cm
#define THRESHOLD_DISTANCE 11
// total number of trigger which will set the flag
#define IR_TRIGGER 3

// Initialize the IR module, set up analog pins
void InitIR(void);

// read from sensor and calculate distance
// This method has a buffer size of 5
float Read(int n);

// read from sensor and calculate distance
// This method has a buffer size of 10
float Read_raw(int n);

// set up timer 0 for counter
void ConfigTimer0(void);

// set up timer interrupt
void ConfigInterrupt(void);

// provide a IR interrupt service
// Check whether there is an obstacle in the front
char IR_ISR(void);

#endif	/* IRADC_H */

