#include <avr/io.h>
#include <util/delay.h>



// vars used with IR transmission
uint16_t timerCounter = 0;
uint8_t  receiveCharCounter = 0;
uint8_t receiveChar = 0;
boolean receivingChar = false; 

// Is it running at 56KHz? (no means 34 KHz)
#define FREQ_56KHz 0

// PWM TOP, used in PWM (generates frequency for the LED) 36=56khz 58=34khz
#define PWMTOP_56 36
#define PWMTOP_34 58
#define PWMTOP_RATIO 1.65

// Timings for detection of bits
#define BIT_BASEVALUE 500

#define BIT_LOW			 BIT_BASEVALUE
#define BIT_HIGH		(BIT_BASEVALUE*2)
#define BIT_LOW_PAR		(BIT_BASEVALUE*3)
#define BIT_HIGH_PAR	(BIT_BASEVALUE*4)
#define BIT_START		(BIT_BASEVALUE*5)

// Defines for use in detectBit() switch cases
#define BITTYPE_LOW			0
#define BITTYPE_HIGH		1
#define BITTYPE_LOW_PAR		2
#define BITTYPE_HIGH_PAR	3

// Timer2 overflow
ISR(TIMER2_OVF_vect) {	
	if (timerCounter < 30000) {
		timerCounter++; // counts amount of overflows (since last reset) to max of BIT_START
	}
}

// Pin interrupt
ISR(PCINT2_vect) {
	// TODO: Replace with flag and call function in loop (?)
	detectBit();
}

int main(void)
{
	Serial.begin(9600);
	IR_initPWM();
	IR_initInterrupt();
	while (1)
	{
		
	}
}

void IR_initPWM()
{
	TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare A
	TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler 1 and PWM, Phase and Frequency Correct
	
	// Define top to run on certain frequency
	if (FREQ_56KHz) {
		OCR2A = PWMTOP_56;
		//OCR2B = PWMTOP_56 / 2;
	} else {
		OCR2A = PWMTOP_34;
		//OCR2B = PWMTOP_34 / 2;
	}
	
	DDRD |= (1 << DDD3); //set OC1A as output PORTB1 pin9
	TIMSK2 = (1 << TOIE2);
	sei();
}

void IR_initInterrupt() {
	PCICR |= (1<<PCIE2);	// Set pin-change interrupt for pins
	PCMSK2 |= (1<<PCINT20);	// Set mask to PCINT20 (4/PD4)
	sei();
}

// Should be called in the interrupt the IR Receiver is connected to
void detectBit() {
	switch (detectBitType(timerCounter)) {
		
		 case BITTYPE_LOW : // Received bit: 0
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			Serial.print(0);
			break;
		case BITTYPE_HIGH : // Received bit: 1
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			receiveChar |= 1; // set LSB to 1
			Serial.print(1);
			break; 
		case BITTYPE_LOW_PAR : // Received bit: 0 (parity)
			if (receiveCharCounter == 0) {
				Serial.print("Stop");
			} else {
				Serial.print("P:0 ");
				Serial.print((char) receiveChar);
			}
			break; 
		case BITTYPE_HIGH_PAR : // Received bit: 1 (parity)
			if (receiveCharCounter == 0) {
				Serial.print("Start");
			} else {
				Serial.print("P:1 ");
				Serial.print((char) receiveChar);
			}
		default: // Received bit: Start
		receiveCharCounter = 0;
	}
	
	Serial.print(" \tTimer: ");
	Serial.println(timerCounter);
	
	timerCounter = 0; // reset timer counter
}

/* Detects bit type, used in detectBit()
 * Returns the int it returns determines what type according to the following:
 *	bit type		value		Timing			Returns
 *	BIT_LOW			0			    - 1.5		0
 *	BIT_HIGH		1			1.5 - 2.5		1
 *	BIT_LOW_PAR		0 (parity)	2.5 - 3.5		2
 *	BIT_HIGH_PAR	1 (parity)	3.5 - 4.5		3
 *  BIT_START		start		4.5 +			4
 */
int detectBitType(double counter) {
	
	if (FREQ_56KHz) {
		counter = counter / PWMTOP_RATIO;
	}
	
	counter = counter - 0.5 * BIT_BASEVALUE;
	counter = counter / BIT_BASEVALUE; 
	return (int) counter;
}
