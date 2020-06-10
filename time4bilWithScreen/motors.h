#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool
#include "utils.h"


/* Pin definition for motors
Left wheel
33 PMD7/RE7 forward
32 PMD6/RE6 backward

Right wheel
31 PMD5/RE5 forward
30 PMD4/RE4 backward
*/

void leftWheelForward(){
    PORTESET = 0x80;
    PORTECLR = 0x40; //the backward movement must be cancelled to avoid paralysis
}
void leftWheelStop(){
    PORTECLR = 0x80; //cancel forward
    PORTECLR = 0x40; //cancel backward
}
void leftWheelBackward(){
    PORTESET = 0x40;
    PORTECLR = 0x80; //forward movement cancelled
}
void rightWheelForward(){
    PORTESET = 0x20;
    PORTECLR = 0x10; //backward movement cancelled
}
void rightWheelStop(){
    PORTECLR = 0x20; //cancel right forward
    PORTECLR = 0x10; //cancel right backward
}
void rightWheelBackward(){
    PORTESET = 0x10;
    PORTECLR = 0x20; //forward movement cancelled
}
void bothWheelsForward(){
    leftWheelForward();
    rightWheelForward();
}

void bothWheelsStop(){
    leftWheelStop();
    rightWheelStop();
}

void bothWheelsBackward(){
    leftWheelBackward();
    rightWheelBackward();
}

bool carIsOn(){
  return isBitSet(PORTD,8);
}

bool carGoesForward(){
  return isBitSet(PORTD,9);
}

void turnLeft(){
    leftWheelStop();
    rightWheelForward();
}
void turnRight(){
    rightWheelStop();
    leftWheelForward();
}

#endif