/* 
 * File:   main.c
 * Author: JayZh
 *
 * Created on November 3, 2021, 9:09 AM
 */

#include "ConfigRegsPIC18F452.h"
#include "serial.h"
#include "IR_Reader.h"
#include "pwm.h"
#include "Object_Avoidance.h"
#include "AutoTraverse.h"
#include "PID.h"

//Install Interrupts
void high_isr(void);
void low_isr(void);
#pragma code high_vector = 0x08
void high_interrupt (void){
    _asm
    goto high_isr 
    _endasm
}

#pragma code low_vector = 0x18
void low_interrupt (void){
    _asm 
    goto low_isr
    _endasm
}
/*
 * 
 */
#pragma code


extern char emergency_stop_flag; //Global Emergency stop flag
char ob_flag = 0; //Obstacle exist flag
char auto_flag = 0; //Switch to auto mode flag
char timer0_flag = 0;   //Timer 0 interrupt function flag
                        //0 for detection, 1 for counting
void main(void) {
    int autoSteer; //Motor steering output from auto mode
    USART_Init(); //Enable SCI, baud rate 9600
    ConfigTimer0(); //Configure timer 0
    PID_Setup(); //Set up PID parameters
    InitIR(); //Initialize IR sensors
    Timer1_Setup(); //Set up timer 1
    MotorSetup(255); //Initialize Motors
    EnableMotor(); //Enable motors 
    SetSpeed(0,0); //Set initial motor speed as 0
    SetDirection(1,1); //Set initial directions forward
    Interrupt_enable(); //Enable interrupts 
    
    for(;;)
    {
        while(1){
            //If do not need to change to auto mode (manual mode)
            if (auto_flag == 0)
            {
                //Received message
                while(PIE1bits.RCIE == 1);  
                
                //If there is an obstacle in the front
                if (ob_flag)
                {
                    //tx out stop sending new message feedback
                    tx_mssg(0);
                    //clear buffer
                    EmptyQ();
                    break;
                }
            }
            else //Change to auto mode
            {
                //Disable timer 0 interrupt
                INTCONbits.TMR0IE = 0;
                //Until new message received to change to manual mode
                while(PIE1bits.RCIE == 1) 
                {   
                    //Perform auto mode
                    autoSteer = WallFollowing();
                    AutoMapping(150, autoSteer);
                }
                //Re-enable timer 0 interrupt
                INTCONbits.TMR0IE = 1;
            }
            //Extract message and execute corresponding action
            auto_flag = cmd_action ();
        }
        
        //If there is an obstacle, perform avoidance state
        if(ob_flag)
        {
            PIE1bits.RCIE = 1;
            timer0_flag = 1; //Timer 0 now used as a time counter
            T0CONbits.T08BIT = 0; //16-bits
            T0CONbits.T0PS2 = 1;
            T0CONbits.T0PS1 = 0; //1:32 prescale
            T0CONbits.T0PS0 = 0;
            avoidance(); //Perform avoidance
            ob_flag = 0; //Clear obstacle flag
            timer0_flag = 0; //Change timer 0 back to trigger constant interrupt
            T0CONbits.T08BIT = 1; //8-bits
            T0CONbits.T0PS1 = 1; //1:256 prescale
            T0CONbits.T0PS1 = 1; //1:256 prescale
            T0CONbits.T0PS0 = 1;
            INTCONbits.TMR0IE = 1;
            //If no emergency stop
            if (emergency_stop_flag !=1)
            {
                tx_mssg(1); //send feedback to ask for new command
            }
            
        }
       
    }  
}

#pragma code
#pragma interrupt high_isr
void high_isr(void){
    
    //if it is serial receive interrupt 
    if (PIE1bits.RCIE && PIR1bits.RCIF){
        rx232Isr();
    }
   
}



#pragma code
#pragma interrupt low_isr
void low_isr(void){
    
    //If it is timer 0 overflow interrupt
    if (INTCONbits.TMR0IF){
        //if timer 0 is used to run IR detection
        if (timer0_flag == 0)
        {
            ob_flag=IR_ISR();
        }
        else //Timer 0 is used as a timer counter
        {
            Timer0_overflow_counter();
        }
        
    }
}