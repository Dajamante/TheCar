
#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool

void lookForward(){
    OC1RS = 469; // set pulse width to 1.5 ms to center servo, ie ((80e6 / 256) / 50)*(1.5/20)
}

void lookLeft(){
    OC1RS = 310*2;
}

void lookRight(){
    OC1RS = 310;
}

#endif