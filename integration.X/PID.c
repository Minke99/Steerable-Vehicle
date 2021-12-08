/* 
 * PID Controller:   PID control for distance / heading
 *                  of robots
 * File:   PID.c
 * Author: Jiashu Wu
 * Last updated on 19 Nov 2021, 3:03 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p18f452.h"

#include "PID.h"
#include "pwm.h"

/* 
 Global Variables
*/

// Previous error calculated
float gradientErrorPrev;
float displacementErrorPrev;

// Integral component
float integralErrorGradient;
float integralErrorDisplacement;

// Differential component
float diffErrorGradient;
float diffErrorDisplacement;

// Gains for the two controllers
float K_i_grad;
float K_d_grad;
float K_p_grad;

float K_i_disp;
float K_d_disp;
float K_p_disp;

// Integral threshold to avoid windup
int integralThreshold;
float PIDRefreshRate;


int Gradient_PID(float currentGradient){

    int steeringParam;
    float ut;
    float steeringCap = 100;

    // Add the previous error to the cumulative sum
    integralErrorGradient += currentGradient;

    // Calculate the differential
    diffErrorGradient = (currentGradient - gradientErrorPrev);

    // Calculate the control output ut
    ut = K_p_grad * currentGradient + K_i_grad * integralErrorGradient + K_d_grad * diffErrorGradient;
    
    // limits ut to 0-250
    if (ut > steeringCap){
        ut = steeringCap;
    }

    else if (ut < -1 * steeringCap){
        ut = -1 * steeringCap;
    }
    ut += 125;
    
    // Convert ut as an integer
    steeringParam = (int) (ut);
    
    // Set the previous error
    gradientErrorPrev = currentGradient;

    // return the calculated parameter 
    return steeringParam;
}

int Displacement_PID(int currentError){

    int steeringParam;
    signed char steeringCap = 100;
    float ut;
    
    // Add the previous error to the cumulative sum
    integralErrorDisplacement += currentError;

    // Calculate the differential
    diffErrorDisplacement = (currentError - displacementErrorPrev);

    // Calculate the control output ut
    ut = 150 * K_p_disp * currentError + K_i_disp * integralErrorDisplacement + K_d_disp * diffErrorDisplacement;

    // limits ut to 0-250
    if (ut > steeringCap){
        ut = steeringCap;
    }

    else if (ut < -1 * steeringCap){
        ut = -1 * steeringCap;
    }
    
    ut += 125;         
    
    // Concatenate ut to an integer
    steeringParam = (int) (ut); // !!!!!!!!!!!! The exact formula need to be altered
    
    // Set the previous error
    displacementErrorPrev = currentError;
    
    // Return the steering parameter
    return steeringParam;
}

void PID_Setup(){

    integralErrorDisplacement = 0;
    integralErrorGradient = 0;
    
    gradientErrorPrev = 0;
    displacementErrorPrev = 0;
    
    K_i_grad = 0.1;
    K_d_grad = 20;
    K_p_grad = 100;
    
    K_i_disp = 0.05;
    K_d_disp = 3;
    K_p_disp = 10;
    
    integralThreshold = 500;
    PIDRefreshRate = 0.1;
}