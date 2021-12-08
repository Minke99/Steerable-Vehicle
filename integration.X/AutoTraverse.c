/* 
 * Auto Mode:    controls vehicle automatically 
 *                  for wall following
 * File:   AutoTraverse.h
 * Author: Jiashu Wu, Jay Zhang
 * Last updated on 19 Nov 2021, 3:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "IR_Reader.h"
#include "PID.h"
#include "pwm.h"



int WallFollowing(void){

    // Definition of gradient: when gradient = 0, wall is parallel to bot
    // gradient < 0,  \|
    // gradient > 0, /|     where the bot's direction is |

    // Variable Definitions

    // PID controller outputs
    int steeringOutputGrad;
    int steeringOutputDisp;
    int steeringOutputAvg;

    // IR sensor raw readings
    float irLeftFront;
    float irLeftRear;

    // Difference between measurements
    int dispError;

    // Desired distance between robot and wall (cm)
    int IRoffset = 10;
    
    // Reading of three sensor outputs
    irLeftFront = Read_raw(2);
    irLeftRear = Read_raw(1);
    dispError = IRoffset - irLeftFront;

    // Maybe another PID here, to turn gradient to 0
    steeringOutputGrad = Gradient_PID(irLeftRear - irLeftFront);
    steeringOutputDisp = Displacement_PID(dispError);
    steeringOutputAvg = (steeringOutputGrad + steeringOutputDisp)/2;
    
    //return steeringOutputGrad;
    return steeringOutputAvg;
}


unsigned char IfObstacle(void){

    // Read the forward IR
    float irForward;

    // If it is less then 10cm to a obstacle in front
    irForward = Read_raw(0);

    // Return 1
    if (irForward < 10){
        return 1;
    }
    else{
        return 0;
    }
}

void AutoTraverse(void){

    // Variable for motor steering
    int autoSteer;

    // Flag for emergency stop
    unsigned char ESTOPFLAG;

    // Set up PID for initialization and gains
    PID_Setup();
    
    // Set the while condition to be triggered upon ESTOP
    while(ESTOPFLAG == 0)
    {   
        if (IfObstacle()){

            AutoMapping(125,125);

        }
        
        else{

            autoSteer = WallFollowing();

            AutoMapping(150, autoSteer);

        }
    }
}