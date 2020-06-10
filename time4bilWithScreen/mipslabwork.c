#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool
#include "i2c.h"
#include "messages.h"


long globalTick = 0;

bool i2CisOn(){ //the vibrations are triggering the accelerometer,
                //that's why we control it with switch 1 at this stage
    if(isBitSet(PORTD,8)) return true;
    else return false;
}

long getGlobalTick(){
  return globalTick;
}

float tickToSec(){
  return getGlobalTick()/50000.0;
}

void wait(float sec){
  float startWaiting = tickToSec();
  float stopWaiting = startWaiting + sec;
  while(tickToSec()<stopWaiting);
}


/* Interrupt Service Routine */
void user_isr(void)
{
  if(flagTimerOneUp()){ //when 100 ms passed
    startTimerFour();
    startTrigger();
    if (i2CisOn()) getData();
  }
  if(flagTimerFourUp()){ //the trigger needs to go up every 100ms, for a period of 10 microsec
      stopTrigger();
      stopTimerFour();
      setFlagTimerOneDown();
      setFlagTimerFourDown();
  }
  if(flagTimerThreeUp()){  //independant counter for the whole project
    globalTick++;
    setFlagTimerThreeDown();
    updateSensor(getGlobalTick()); //an update every 20 micro is enough
  }
}


void labinit(void)
{
    /* MOTORS*/

  // configured on PORTE 7:4, pins 30:33
  TRISE = 0x00;
  /* Sensor and Switches on PORTD */
  TRISD = 0x320;//bit 8 and 9 of port D are inputs (switch 1 and switch 2)
                //Sensor: bit 11 = trigger is output, bit 5 = echo is input

  /* TIMERS */

  //TMR1 handles sending a pulse every 100ms. It has flag: IFS0<4>
  TMR1 = 0x0; // Clear timer register
  PR1 = ((80e6/256)/10); // 100 ms (needs to be > 60ms time out according to the docs.
  T1CONSET = 0x8030;// Start Timer 1, preschaling with bits 5:4 to 1:256

  //flag: IFS0<12>. Global ticker at 20 micro. Cannot use TMR2 because servo
  TMR3 = 0x0;
  PR3 = (80e6 / 64) / 50000;
  IPC(3) = 0x1E; //set high prio
  T3CONSET = 0x8060;

  //TMR4 handles trigger. Flag IFS0<16> Enable IEC0<16> Prio IPC4<4:2> subprio IPC4<1:0>
  TMR4 = 0x0;
  PR4 = 80e6*1e-5; //time out needs to be 10 us, so the period size is 800
                //TMR4 starts in the user_isr routine
  IPC(4) = 0x1F; //set highest priorities lecture 6 for timer 4
                 // TABLE 7-1: INTERRUPT IRQ AND VECTOR LOCATION PIC32 family manual
  IPC(1) = 0x1D; //set slightly lower priority IPC1<4:2> subprio: IPC1<1:0>

  /* SERVO */

  // configure PWM on OC1 (RD4)
  TMR2 = 0x0; //reset timer
  PR2 = (80e6 / 256) / 50; // 20 ms must be 50hz
  T2CONSET = 0x8070; //T2CON must be 0x70 (bits 6 to 4) for prescaling 1:256

  OC1RS = 469; // set pulse width to 1.5 ms to center servo, ie ((80e6 / 256) / 50)*(1.5/20)
  OC1R = 469; // set pulse width to 1.5 ms to center servo to start? Don't understand

  OC1CONSET = 0x8000; //ON: Output Compare Peripheral On bit
  OC1CONSET = 0x6; //  OCM<2:0>: Output Compare Mode Select bits 110 = PWM mode on OCx; Fault pin disabled

  IEC(0) = 0x11010; //enable interrupt IEC0<16> TMR4 enable: IEC0<4> TMR1, IEC0<12> TM3
  enable_interrupt();

  if(i2CisOn()) i2cinit();
  return;
}
/* This function is called repetitively from the main program */


void labwork(void)
{
  if(readSensor()<=7){
    bothWheelsStop();
    display_update();
    lookLeft();
    wait(1);

    if(!(readSensor()<=7)){
      turnLeft();
      wait(1);
      bothWheelsForward();
    }else{
      bothWheelsStop();
      lookRight();
      display_update();
      wait(1);
      if(!(readSensor()<=7)){
        turnRight();
        wait(1);
        bothWheelsForward();
        display_update();
        } else {
          bothWheelsBackward();
          wait(2);
          leftWheelForward();
          rightWheelBackward();
          wait(1);
          bothWheelsForward();
        }
    }
  }else{
    lookForward();
    bothWheelsForward();
    if(!i2CisOn()) rollingHating(tickToSec());
    }
}