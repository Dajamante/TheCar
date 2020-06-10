#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool

long echoStart = 0;
long echoStop = 0;
bool echoHasBeenTriggered = false;
bool takeStartTime = false;
long globalDistance = 0;

/*
35 IC4/PMCS1/PMA14/INT4/RD11 *trig
34 PMRD/CN14/RD5 * echo
*/

void startTrigger(){
  PORTDSET = 0x800; //sets bit 11, starts the trigger
  takeStartTime = true;
}

void stopTrigger(){
  PORTDCLR = 0x800; //clear bit 11 of port D, ie stop the trigger
}

bool echoIsUp(){
  return (PORTD & 0x20);
}

long calculateDistance(){
  return ((echoStop-echoStart)*5)/58; //5* == little hack 1/58 == 34000/2*10^-6
}

void updateSensor(long tick){
  if(takeStartTime){
    echoStart= tick;
    takeStartTime= false;
  }
  if(echoIsUp()){
      echoStop = tick;
      echoHasBeenTriggered = true;
    }
  if(echoHasBeenTriggered && !echoIsUp()){
    echoHasBeenTriggered = false;
    globalDistance = calculateDistance();
  }
}

long readSensor(){
  return globalDistance;
}


#endif
