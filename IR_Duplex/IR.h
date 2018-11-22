/*
  IR.h

  Created: 11/22/2018 11:47:35 AM
  Author: Jacco
  Author: Christiaan
*/

#ifndef IR_H
#define IR_H

#include "Arduino.h"

// Maximum message size (in bytes)
#define MESSAGE_SIZE 8

// PWM TOP, used in PWM (generates frequency for the LED) 36=56khz 58=34khz
#define PWMTOP_56 36      //(16.000.000/8/56.000)=36
#define PWMTOP_38 53      //(16.000.000/8/38.000)=53
#define PWMTOP_RATIO 1.47 //(56.000/38.000)=1.47

// Defines for sending data
#define VAL_LOW 1
#define VAL_HIGH 2
#define VAL_LOW_PAR 3
#define VAL_HIGH_PAR 4

// Defines for receiving data
#define BIT_LOW	BIT_BASEVALUE
#define BIT_HIGH (BIT_BASEVALUE*2)
#define BIT_LOW_PAR (BIT_BASEVALUE*3)
#define BIT_HIGH_PAR (BIT_BASEVALUE*4)
#define BIT_START (BIT_BASEVALUE*5)

// Defines for use in detectBit() switch cases
#define BITTYPE_LOW 0
#define BITTYPE_HIGH 1
#define BITTYPE_LOW_PAR 2
#define BITTYPE_HIGH_PAR 3

// Used for debugging: disables pulse mode
#define TURN_PULSEMODE_ON true


class IR
{
  public:
    IR(uint8_t KHz); // KHz can be 56 or 38
    IR(uint8_t KHz, uint8_t Psize); // KHz can be 56 or 38

    void write(uint8_t byteIn[MESSAGE_SIZE]);
    uint8_t available();
    void read(char *string);
    int error();

    //  Functions called on interrupts
    void timerOverflow();
    void pinChange();

  private:
    // private functions
    void IRinit(uint8_t KHz, uint8_t Psize);
    int detectBitType(double counter);
    uint8_t has_even_parity(uint8_t x);

    // settings
    uint8_t BIT_BASEVALUE;
    uint8_t Fq56mode;

    // Variables used for sending data
    uint16_t OVF_counter = 0;				//pulse length counter
    uint16_t Pulse_value = 0;				//value to set the pulse length
    uint8_t TxCode[(MESSAGE_SIZE * 9) + 2];	//array that is used to prepare the to be transmitted message
    uint8_t commandCounter = 0;				//get the right pulse out of the array

    // Variables used for receiving data
    uint16_t timerCounter = 0;
    uint8_t  receiveCharCounter = 0;
    uint8_t receiveChar = 0;
    int receivingChar = 0;

    int dataAvailable = 0;
    uint8_t data[MESSAGE_SIZE + 1];
    uint8_t dataNr; // Counter for which byte is being received & saved

    int parityError = 0; // Set to true when receiving parity errors
};
#endif //__IR_H__
