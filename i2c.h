#ifndef I2C_H
#define I2C_H

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include <stdbool.h> // need this for bool

#define MPU_SENSOR_ADDR 0x68
// #define ADXL345_DEVICE 0x53
int mpu_register = 0x3F;
// int adxl_register = 0x32;

#define zMax50 50
int zValues50[zMax50];
int zIndex50=0;
//this gives a stable mean
#define zMax10 10
int zValues10[zMax10];
int zIndex10=0;
//this gives a fast mean to be compared to the stable mean

#define zOffsetDontLift 500 //empirical tests :)
long lastLift = 0;
long lastDrop = 0;
/* Wait for I2C bus to become idle */
void i2c_idle() {
    //display_string(0, "i'm in idle");
    //display_update();
	while(I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //TRSTAT
}
//I2C1CON bit 0 TBF: Transmit Buffer Full Status bit
//I2C1CON bit 1 RBF: Receive Buffer Full Status bit
//I2C1CON bit 2 R/W: Read/Write Information bit 
//I2C1CON bit 3 Updated when Start, Reset or Stop detected; cleared when the I 2 C module is disabled (ON = 0).
//I2C1CON bit 4 Updated when Start, Reset or Stop detected; cleared when the I 2 C module is disabled (ON = 0).
//TRSTAT: 1 = Master transmit is in progress (8 bits + ACK)

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data){
    //display_string(0, "i'm in send");
    //display_update();
	i2c_idle();
	I2C1TRN = data;
    //display_string(0, "I have your hostage I2C1TRN");
    //display_update();
    //I2CxTRN is the register to which transmit data is written.
    //This register is used when the I 2 C module operates as a
    // master transmitting data to the slave, or as a slave sending
    // reply data to the master. As the message progresses,
    //the I2CxTRN register shifts out the individual bits.
    //As a result, the I2CxTRN register may not
    // be written to unless the bus is idle.
	i2c_idle();
    //display_string(0, "we have passed the second idle");
    //display_update();
	return !(I2C1STAT & (1 << 15)); //ACKSTAT
}


/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
	i2c_idle();
	I2C1CONSET = 1 << 3; //RCEN = 1
	i2c_idle();
	I2C1STATCLR = 1 << 6; //I2COV = 0
	return I2C1RCV;
}


/* Send acknowledge conditon on the bus */
void i2c_ack() {
	i2c_idle();
	I2C1CONCLR = 1 << 5;
    //ACKDT = 0
	I2C1CONSET = 1 << 4;
    //ACKEN: Acknowledge Sequence Enable bit
    //applicable during master receive.
    //1 = Initiate Acknowledge sequence
    // on SDAx and SCLx pins, and transmit
    // ACKDT data bit; cleared by module
}


/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
	i2c_idle();
	I2C1CONSET = 1 << 5;
    //ACKDT: Acknowledge Data bit
    //1 = A NACK is sent
	I2C1CONSET = 1 << 4;
    //ACKEN: Acknowledge Sequence Enable bit
    //applicable during master receive.
    //1 = Initiate Acknowledge sequence
    // on SDAx and SCLx pins, and transmit
    //ACKDT data bit; cleared by module
}

/* Send start conditon on the bus */
void i2c_start() {
	i2c_idle();
	I2C1CONSET = 1 << 0; //SEN: Start Condition Enable bit
	i2c_idle();          //Section 24. Inter-Integrated Circuit™ (I 2 C™)
}

/* Send restart conditon on the bus */
void i2c_restart() {
	i2c_idle();
	I2C1CONSET = 1 << 1; //RSEN: Restart Condition Enable bit
	i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
	i2c_idle();
	I2C1CONSET = 1 << 2; //PEN: Stop Condition Enable bit
	i2c_idle();
}

void printData(int zAverage10, int zAverage50){
    if(zAverage10 <= (zAverage50 - zOffsetDontLift)){
        lastDrop = tickToSec();
        if(tickToSec()-lastLift > 2.0){
            display_string(0, "Thank you!");
            lastLift = 0;
        }
     }
    else if(zAverage10 > (zAverage50 + zOffsetDontLift)){
        lastLift = tickToSec();
        if(tickToSec()-lastDrop>2.0){
            display_string(0, "DON'T LIFT ME!");
            lastDrop = 0;

        }
    }


    else display_string(0,"");

    display_string(1, "Mean10 & Mean50:");
    display_string(2, itoaconv(zAverage10));
    display_string(3,itoaconv(zAverage50));
    display_update();
}

void getData(){
    int AcZ ;

    /* This is done by the master starting
    off the transmission in a similar fashion
     as the write, by sending the address with the R/W
     bit equal to 0 (signifying a write).... */
    do {
        i2c_start();
    } while(!i2c_send(MPU_SENSOR_ADDR << 1));
    //display_string(0,"we got get data started");
    display_update();

    /* ... followed by the register address it wishes to read from... */
    i2c_send(mpu_register);

    //display_string(0,"we have sent mpu register");
    display_update();

    /* Once the slave acknowledges this register address,
     the master will send a START condition again,
     followed by the slave address with the R/W bit
     set to 1 (signifying a read) */
    do {
        i2c_start();
        //display_string(0,"slave sent read condition");
        display_update();
    } while(!i2c_send((MPU_SENSOR_ADDR << 1) | 1));

    AcZ = ((int)i2c_recv()<<8);
    i2c_ack();
    AcZ |= (int)i2c_recv();
    i2c_nack();
    i2c_stop();

    //display_string(0, "all data arrived");

    zValues50[zIndex50%zMax50]=AcZ; //going round and putting the last result first,
                                    // ex 11 will go to index 1

    zValues10[zIndex10%zMax10]=AcZ;
    zIndex50++;
    zIndex10++;

    int zAverage50 = 0;
    int zAverage10 = 0; //it is not a good idea to compare the average to a
                        //non-optimized AcZ. Average10 is an attempt to get a
                        //stabilized but fast value

    int i=0;
    for(i = 0; i < zMax50 ; i++){
        zAverage50 += zValues50[i];
    }
    int j=0;
    for(j=0; j<zMax10;j++){
        zAverage10 += zValues10[j];
    }
    zAverage50 /= zMax50;
    zAverage10 /= zMax10;

    printData(zAverage10, zAverage50);
}

void i2cinit(){
    /* Set up i2c */
	I2C1CON = 0x0;
	/* I2C Baud rate should be less than 400 kHz, is generated by dividing
	the 40 MHz peripheral bus clock down */
	I2C1BRG = 0x0C2;
	I2C1STAT = 0x0;
	I2C1CONSET = 1 << 13; //SIDL = 1
	I2C1CONSET = 1 << 15; // ON = 1
	char clear = I2C1RCV; //Clear receive buffer

    //display_string(0, "clear buffert");
    display_update();
  /* Send start condition and address of the temperature sensor with
	write mode (lowest bit = 0) until the temperature sensor sends
	acknowledge condition */

	do {
		i2c_start();
        //display_string(0, "trying to start i2c");
        display_update();
	} while(!i2c_send(MPU_SENSOR_ADDR << 1));
	/* Send register number we want to access */

    i2c_send(0x6B);
    //i2c_send(0x2D); //adxl345
    //display_string(0, "power bit received");
    display_update();
	/* Set the config register to 0 */
	i2c_send(0x0);
    //display_string(0, "send stop condition");
    display_update();
	/* Send stop condition */
	i2c_stop();
    //display_string(0, "i2c stop");
    display_update();
}
#endif