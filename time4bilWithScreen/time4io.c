#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw(){
    volatile int *portD = PORTD;
    return (*portD>>8) & 0xF; //switcharna
}

int getbtns(){
    volatile int *portD = PORTD;
    return (*portD>>5) & 0x7; //knapparna

}