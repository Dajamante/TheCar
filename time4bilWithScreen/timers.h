#pragma once
// Om den ovan inte funkar så använd (se öven #end if på slutet av filen):
#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool

/**********************************************************************************************
Timer routines
**********************************************************************************************/

void startTimerOne(){
  TMR1= 0x0;
  T1CONSET = 0x8000;
}

void startTimerFour(){
  TMR4 = 0x0;
  T4CONSET = 0x8000;
}

void stopTimerOne(){
  T1CONCLR = 0x8000;
}
void stopTimerFour(){
  T4CONCLR = 0x8000;
}

/**********************************************************************************************
Flag routines
**********************************************************************************************/

bool flagTimerOneUp(){
  return IFS(0) & 0x10;
}

void setFlagTimerOneDown(){
  IFS(0) &= ~0x10;
}

bool flagTimerFourUp(){
  return IFS(0) & 0x10000;
}

void setFlagTimerFourDown(){
  IFS(0) &= ~0x10000;
}

bool flagTimerThreeUp(){
  return IFS(0) & 0x1000;
}

void setFlagTimerThreeDown(){
  IFS(0) &= ~0x1000;
}

#endif