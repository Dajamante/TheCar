#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool


bool isBitSet(int port, int bit){
  return ((port>>bit) & 0x1) == 0x1;
}





#endif