#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool
#include "motors.h"
#include "sensor.h"
#include "servo.h"
#include "utils.h"


bool motorTest(float sec) {
    if(sec < 3) bothWheelsForward();
    else if(sec < 6) bothWheelsStop();
    else if(sec < 9) bothWheelsBackward();
    else if(sec < 12){
        rightWheelForward();
        leftWheelStop();
    }
    else if(sec < 15){
        rightWheelBackward();
        leftWheelStop();
        }
    else if(sec < 18){
        leftWheelForward();
        rightWheelStop();
    }
    else if(sec < 21){
        leftWheelBackward();
        rightWheelStop();
    }else{
        bothWheelsStop();
        return true;
    }
    return false;

}

void servoTest(float sec){

    if(sec < 3) lookLeft();
    else if(sec < 6) lookForward();
    else if(sec < 9) lookRight();
    else lookForward();
}

void timer3Test(float sec){
    if((int)(sec)%3 == 0){
        PORTESET = 0x1;
    }else PORTECLR = 0x1;
}

void sensorTest(long tick){
    long reply = readSensor();
    PORTECLR=0x7;
    if(reply<=5){
        PORTESET = 0x1;
    }
    else if(reply<=10){
        PORTESET = 0x2;
    }
    else if(reply<=15){
        PORTESET = 0x4;
    }
}
void systemTest(long tick){
    float sec = tick/50000;
    motorTest(sec);
    servoTest(sec);
    timer3Test(sec);
    sensorTest(tick);
}

#endif