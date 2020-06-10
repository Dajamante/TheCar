#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool

void rollingHating(float timeInSec){
  int sec = ((int)timeInSec)%10;

  if(sec < 5){
    display_string(2, "They see me ");
    display_string(3, "ROLLIN....");
    display_update();
  }else{
    display_string(2, "THEY HATIIIIN!!!");
    display_string(3, "");
    display_update();
    }
}


void rolling(){
  display_string(2, "They see me ");
  display_string(3, "ROLLIN....");
  display_update();
}

void hatin(){
  display_string(2, "THEY HATIIIIN!!!");
  display_string(3, "");
  display_update();

}

#endif