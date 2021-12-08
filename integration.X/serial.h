/* 
 * File:   serial.h
 * Author: jay
 *
 * Created on 8 September 2021, 10:16 AM
 */

#ifndef SERIAL_H
#define	SERIAL_H


#include "delays.h"
#include "pwm.h"
#include "IR_Reader.h"
#include "p18cxxx.h"


#define MaxSize 15 //Max buffer size
#define max_data_size 15 //Max data size
//Receive circular buffer
struct QNode{
    char Data[MaxSize];
    char rear;
    char front;
    char overwrite_flag;
};
typedef struct QNode *Queue;

//Data buffer
struct extracted_data_buffer{
    char length;
    char data_type;
    unsigned char data[max_data_size];
    unsigned char checksum;
};
typedef struct extracted_data_buffer * Extracted_Data;

//Serial receive interrupt service routine
void rx232Isr(void); 

//Interrupt initialization 
void Interrupt_enable(void);  

//SCI initialization 
void USART_Init(void);  

//Compose message that needs to be transmitted into the package structure
void compose_package(char *dataPtr, char dataLength,char dataType, Queue DestinationPtr);

//Extract package and execute the corresponding action, return 1 if robot need to 
//switch to auto mode
char cmd_action (void);

//Transmit defined messages (Continue, Stop, Xbee Fails)
void tx_mssg(char select);

//Clear the serial receive buffer
void EmptyQ(void);



#endif	/* SERIAL_H */

