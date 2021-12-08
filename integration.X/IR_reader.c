/* 
 * IR_reader: read analog signal from analog pin
 *            Convert analog data back to the distance in float
 *            Check whether there is an obstacle in the front
 * File:   IR_Reader.c
 * Author: 490189050, 490160392
 * Created on 3 Oct 2021, 3:00 PM
 */

#include <p18f452.h>
#include "IR_Reader.h"
#include "PWM.h"

// sensor measurements
float sensor_front;
float sensor_left1;
float sensor_left2;

// Define variables
int distance_IR = 0.0;
double k0 = 12.73;
double b0 = -0.02;
int reading_buff_size = 5;
const int reading_buff_size_raw = 10;

// 1 -> set (object detected), 0 -> Not set  
// counts the number of IR trigger
unsigned char IR_trigger_counter = 0;

// initialize the analog pin
// currently only an0 is enabled
void InitIR()
{
    T0CON = 0b11000111;
    // Enable AN0
    ADCON0 = 0b01000001;
    ADCON1 = 0b00000000; // Left justified
    sensor_front = 0;
    sensor_left1 = 0;
    sensor_left2 = 0;
    IR_trigger_counter = 0;
}

// read from sensor and calculate distance
// This method has a buffer size of 5 
float Read(int n)
{
    float distance;
    float voltage;
    float measurements[5];
    unsigned char isFinish;
    int i;
   
    switch(n){
        case 0:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;

            break;
        case 1:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 2:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            
            break;
    }
    
    // read a double array of distances
    for ( i = 0; i < reading_buff_size; i++) {
        // read from analog
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);

        // convert to distance 
        voltage = ADRESH;
        voltage = (voltage*5.0)/256.0;
        distance = 1.0/((voltage-b0)/k0 + 1/24);
        measurements[i] = distance;
    }
    
    // sort the array
    isFinish = 0;
    while (!isFinish) {
        isFinish = 1;
        for ( i = 0; i<reading_buff_size - 1; i++) {
            if (measurements[i]>measurements[i+1]) {
                isFinish = 0;
                distance = measurements[i];
                measurements[i] = measurements[i+1];
                measurements[i+1] = distance; 
            }
        }
    }
    
    // get the median of the distance
    if (reading_buff_size % 2 == 0) {
        distance = measurements[reading_buff_size/2];
    }
    else {
        distance = measurements[(reading_buff_size-1)/2];

    }
    
    return distance;
}


// read from sensor and calculate distance
// This method has a buffer size of 10
float Read_raw(int n)
{
    float distance;
    float voltage;
    float measurements[10];
    unsigned char isFinish;
    int i;
   
    switch(n){
        case 0:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 0;

            break;
        case 1:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS0 = 1;
            break;
        case 2:
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS0 = 0;
            
            break;
    }
    
    // read a double array of distances
    for ( i = 0; i < reading_buff_size_raw; i++) {
        // read from analog
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1);

        // convert to distance 
        voltage = (ADRESH*5.0)/256.0;
        distance = 1.0/((voltage-b0)/k0 + 1/24);
        measurements[i] = distance;
    }
    
    // sort the array
    isFinish = 0;
    while (!isFinish) {
        isFinish = 1;
        for ( i = 0; i<reading_buff_size_raw - 1; i++) {
            if (measurements[i]>measurements[i+1]) {
                isFinish = 0;
                distance = measurements[i];
                measurements[i] = measurements[i+1];
                measurements[i+1] = distance; 
            }
        }
    }
    
    // get the median of the distance
    if (reading_buff_size_raw % 2 == 0) {
        distance = measurements[reading_buff_size_raw/2];
    }
    else {
        distance = measurements[(reading_buff_size_raw-1)/2];

    }
    
    return distance;
}

// set up timer interrupt
void ConfigInterrupt(void)
{
    INTCONbits.GIEH = 0;        // disable interrupts
    INTCONbits.GIEL = 0;
    RCONbits.IPEN = 1;
    
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCON2bits.TMR0IP = 0;
    
    INTCONbits.GIEH = 1;        // enable interrupts
    INTCONbits.GIEL = 1;
}

// set up timer 0 for counter
void ConfigTimer0(void) {
    //  8 bit mode, prescale = 256
    T0CON = 0b11000111;    
}

// provide a IR interrupt service
// Check whether there is an obstacle in the front
char IR_ISR(void) {
    // Read the IR distance
    INTCONbits.TMR0IF = 0;
    
    sensor_front = Read(0);
    
    // check distance saved
    if (sensor_front <= THRESHOLD_DISTANCE) {
        IR_trigger_counter += 1;
    }
    else {
        IR_trigger_counter = 0;
    }
    
    if (IR_trigger_counter >= IR_TRIGGER) {
        IR_trigger_counter = 0;
        INTCONbits.TMR0IE = 0;
        SetSpeed(0,0);
        return 1;
    }
    
    return 0;
}