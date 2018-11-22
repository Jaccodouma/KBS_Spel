/* 
* IR.cpp
*
* Created: 11/22/2018 11:47:34 AM
* Author: Jacco
*/


#include "IR.h"

// default constructor
IR::IR(int KHz) {
	while (KHz != 56 && KHz != 38); // invalid frequency
	
	// Vars
	dataAvailable = 0;
	dataNr = 0;
	
	// PWM
	TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare B
	TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler to 8 and Fast PWM
	if (KHz = 56) {
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
	PCICR |= (1<<PCIE2);	// Set pin-change interrupt for pins
	PCMSK2 |= (1<<PCINT20);	// Set mask to PCINT20 (4/PD4)
	sei();
} //IR


void IR::write(uint8_t byteIn[MESSAGE_SIZE]) {    //transform the massage into a "array with pulses"
	uint8_t x = 0;
	uint8_t bitNr = 0;

	TxCode[bitNr] = 4;
	bitNr++;
	TxCode[bitNr] = 5;
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
		TxCode[bitNr] = 5;
		bitNr++;
		x++;
	}
	TxCode[bitNr] = 3;
	bitNr++;
	TxCode[bitNr] = 0; //a zero is needed to end the pulse code message
	Pulse_value = 1; //start the transmission
}

int IR::available() {
	return dataAvailable;
}

void IR::read(char *string) {
	data[dataNr+1] = '\0';
	strcpy(string, *data);
}

int IR::error() {
	return IR::parityError;
}

int IR::detectBitType(double counter) {
	if (FREQ_56KHz) {
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
			if (FREQ_56KHz) {
				Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE * PWMTOP_RATIO;
				} else {
				Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE;
			}
			commandCounter++;
		} else {
			Pulse_value = 0;
			commandCounter = 0;
			TCCR2A &= ~(1 << COM2B1);
		}
	}
	OVF_counter++; //add 1 every time the timer reaches the top (PWMTOP)
}

void IR::pinChange() {
	switch (IR::detectBitType(timerCounter)) {
		case BITTYPE_LOW : // Received bit: 0
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			break;
		case BITTYPE_HIGH : // Received bit: 1
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			receiveChar |= 1; // set LSB to 1
			break;
		case BITTYPE_LOW_PAR : // Received bit: 0 (parity)
			if (receiveCharCounter == 0) { // Stop bit
				dataAvailable = true;
			} else {
				if (has_even_parity(receiveChar)) {
					// Data is received properly
					data[dataNr] = receiveChar;
				} else {
					// parity error
					parityError = true;
				}
			}
			break;
		case BITTYPE_HIGH_PAR : // Received bit: 1 (parity)
		if (receiveCharCounter == 0) { // Start bit
			dataAvailable = false;
			// empty data
		} else {
			if (!has_even_parity(receiveChar)) {
				// data is received properly
				data[dataNr] = receiveChar;
			} else {
				// Parity error
				parityError = true;
			}
			break;
		}
		default: // Received bit: Start
		receiveCharCounter = 0;
		receiveChar = 0;
	}
	timerCounter = 0; // reset timer counter
}

/*
ISR(TIMER2_OVF_vect) {
	IR::timerOverflow();
}

ISR(PCINT2_vect) {
	IR::pinChange();
}*/