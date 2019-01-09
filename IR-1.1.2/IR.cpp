/*
  IR.cpp

  Created: 11/22/2018 11:47:34 AM
  Author: Jacco
  Author: Christiaan
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "IR.h"

// constructor
IR::IR(uint8_t KHz) {
  IR::IRinit(KHz, 30); //30 is the default value
}

IR::IR(uint8_t KHz, uint8_t Psize) {
  IR::IRinit(KHz, Psize);
}

void IR::IRinit(uint8_t KHz, uint8_t Psize) {
  while (KHz != 56 && KHz != 38); // invalid frequency

  if (KHz == 56) {
    Fq56mode = 1;
  } else {
    Fq56mode = 0;
  }

  // Vars
  BIT_BASEVALUE = Psize;
  dataAvailable = 0;

  // PWM
  TCCR2A = (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare B
  TCCR2B = (1 << CS21) | (1 << WGM22); //set clock prescaler to 8 and Fast PWM
  if (Fq56mode) {
    OCR2A = PWMTOP_56;
    if (TURN_PULSEMODE_ON) { //turn the 34 or 56 KHz PWM mode ON or OFF (debug only)
      OCR2B = PWMTOP_56 / 2; //50% duty cycle
    } else {
      OCR2B = PWMTOP_56 ;    //100% duty cycle (without any pulses)
    }
  } else {
    OCR2A = PWMTOP_38;
    if (TURN_PULSEMODE_ON) { //turn the 34 or 56 KHz PQM mode ON or OFF (debug only)
      OCR2B = PWMTOP_38 / 2; //50% duty cycle
    } else {
      OCR2B = PWMTOP_38;    //100% duty cycle (without any pulses)
    }
  }
  DDRD |= (1 << DDD3); //pin d3 OUTPUT
  TIMSK2 = (1 << TOIE2); //turn overflow interrupt on
  TCCR2A &= ~(1 << COM2B1); //pin d3 LOW

  // Interrupt
  PCICR = (1 << PCIE2);  // Set pin-change interrupt for pins
  PCMSK2 = (1 << PCINT20); // Set mask to PCINT20 (4/PD4)
  sei();
} //IR

float IR::getByteRate() {
  byteRate = ((message_size * 100) / ((time_ms - speed_timer) / 10)); //update the byteRate value
  speed_timer = time_ms;
  message_size = 0;
  if (byteRate > 0 && byteRate < 200) {
    return byteRate;
  }
  return 0;
}

long IR::getTime_ms() {
  return time_ms;
}


void IR::write(uint8_t byteIn[MESSAGE_SIZE]) {    //transform the massage into a "array with pulses"
  StopRec = 1; //disables the receiver
  uint8_t x = 0;
  uint8_t bitNr = 0;

  TxCode[bitNr] = 4; //the start bit
  bitNr++;

  while (byteIn[x] > 0) {
    for (uint8_t n = 8; n > 0; n--) { //set the pulse lengths in the array
      if (byteIn[x] & (1 << (n - 1))) {
        TxCode[bitNr] = VAL_HIGH;
      } else {
        TxCode[bitNr] = VAL_LOW;
      }
      bitNr++;
    }
    if (has_even_parity(byteIn[x])) { //calculate the parity bit
      TxCode[bitNr] = VAL_LOW_PAR;     //write a 0 parity
    } else {
      TxCode[bitNr] = VAL_HIGH_PAR;    //write a 1 parity
    }
    bitNr++;
    x++;
  }
  TxCode[bitNr] = 3; //the stop bit
  bitNr++;
  TxCode[bitNr] = 1; //just te make sure the message is even
  bitNr++;
  TxCode[bitNr] = 0; //a zero is needed to end the pulse code message
  Pulse_value = 1; //start the transmission
}

uint8_t IR::available() {
  if (dataAvailable && ((((timerCounter / PWMTOP_RATIO) > RECEIVE_DELAY) && Fq56mode) || ((timerCounter > RECEIVE_DELAY) && !Fq56mode))) { //wait a few overflows
    dataAvailable = 0;
    return 1;
  } else {
    return 0;
  }
}

/*void IR::read(uint8_t *stream) {
 // uint8_t n = 0;
 // while (data[n] > 0) {
 //   stream[n] = data[n];
 //   n++;
 // }

// for(int x= 0; x < 3; x++){
 // stream[x] = data[x];
// }
strcpy(stream, data);
}*/

int IR::error() {
  if (parityError) {
    uint8_t x = parityError;
    parityError = 0;
    return x;
  } else {
    return 0;
  }
}

int IR::detectBitType(double counter) {
  if (Fq56mode) {
    counter = counter / PWMTOP_RATIO;
  }

  counter = counter - 0.5 * BIT_BASEVALUE;
  counter = counter / BIT_BASEVALUE;
  return (int) counter;
} //detectBitType

uint8_t IR::has_even_parity(uint8_t x) { //check the parity
  uint8_t count = 0, i, b = 1;
  for (i = 0; i < 8; i++) {
    if ( x & (b << i) ) {
      count++;
    }
  }
  if ( (count % 2) ) {
    return 0;
  } else {
    return 1;
  }
}

void IR::timerOverflow() {
  // Timer used for receiving data
  if (timerCounter < 30000) {
    timerCounter++; // counts amount of overflows (since last reset) to max of 30000
  }

  // Sending data
  if (OVF_counter > Pulse_value && Pulse_value > 0) { //
    OVF_counter = 0;
    TCCR2A ^= (1 << COM2B1);

    if (TxCode[commandCounter] > 0) {
      if (Fq56mode) {
        Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE * PWMTOP_RATIO;
      } else {
        Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE;
      }
      commandCounter++;
    } else {
      Pulse_value = 0;
      commandCounter = 0;
      StopRec = 0; //enables the receiver
      TCCR2A &= ~(1 << COM2B1);
    }
  }
  OVF_counter++; //add 1 every time the timer reaches the top (PWMTOP)
  timer_ovf++;
  if (Fq56mode) {
    if (timer_ovf >= 58) {
      time_ms++;
      timer_ovf = 0;
    }
  } else {
    if (timer_ovf >= 38) {
      time_ms++;
      timer_ovf = 0;
    }
  }
}

void IR::pinChange() {
  if (!StopRec) {
    switch (IR::detectBitType(timerCounter)) {
      case BITTYPE_LOW : // Received bit: 0
        receiveChar = receiveChar << 1; // Shift receiveChar
        receiveCharCounter++;
        break;
      case BITTYPE_HIGH : // Received bit: 1
        receiveChar = receiveChar << 1; // Shift receiveChar
        receiveCharCounter++;
        receiveChar |= 1; // set LSB to 1
        break;
      case BITTYPE_LOW_PAR : // Received bit: 0 (parity)
        if (receiveCharCounter == 0) { // Stop bit
          for (uint8_t n = dataNr; n <= MESSAGE_SIZE; n++) { //clear the unused positions in the array
            data[n] = 0;
          }
          message_size += dataNr;
          dataAvailable = true; //the freshly received array is ready ;)
        } else {
          if (has_even_parity(receiveChar)) {
            // Data is received properly
            data[dataNr] = receiveChar;
            receiveCharCounter = 0;
            receiveChar = 0;
            dataNr++;
          } else {
            // parity error
            parityError = true; //Oops an error accor...
          }
        }
        break;
      case BITTYPE_HIGH_PAR : // Received bit: 1 (parity)
        if (receiveCharCounter == 0) { // Start bit
          dataAvailable = false;
          dataNr = 0;
          // empty data
        } else {
          if (!has_even_parity(receiveChar)) {
            // data is received properly
            data[dataNr] = receiveChar;
            receiveCharCounter = 0;
            receiveChar = 0;
            dataNr++;
          } else {
            // Parity error
            parityError = true; //Oops an error accor...
          }
          break;
        }
      default: // Received bit: Start
        receiveCharCounter = 0;
        receiveChar = 0;
    }
    timerCounter = 0; // reset timer counter
  }
}
