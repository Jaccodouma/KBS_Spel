#include <avr/io.h>
#include <util/delay.h>

#define PWMTOP 36 

uint16_t timerCounter = 0;
uint8_t  receiveCharCounter = 0;
char receiveChar = 0;


// Timings for detection of bits
#define BIT_BASEVALUE 1000

#define BIT_LOW			 BIT_BASEVALUE
#define BIT_HIGH		(BIT_BASEVALUE*2)
#define BIT_LOW_PAR		(BIT_BASEVALUE*3)
#define BIT_HIGH_PAR	(BIT_BASEVALUE*4)
#define BIT_START		(BIT_BASEVALUE*5)

// Send and receive frequencies
#define FREQ_SEND 34
#define FREQ_REC 56

// Timer2 overflow
ISR(TIMER2_OVF_vect) {	
	timerCounter++; // counts amount of overflows (since last reset)
}

// Pin interrupt
ISR(PCINT2_vect) {
	// TODO: Check if time since last is long enough to be the start bit
	if (receiveCharCounter == 9) {
		// Parity bit 
		receiveCharCounter = 0;
		Serial.print("Parity bit: ");
		if ( abs(timerCounter - BIT_HIGH_PAR) > abs(timerCounter - BIT_LOW_PAR) ) {
			// Time since last interrupt is time for parity 1
			Serial.println("1");
		} else {
			// Time since last interrupt is time for parity 0
			Serial.println("0");
		}
	} else if (receiveCharCounter != 0) {
		/* Do nothing at 0, this is where we have to start counting so
		 * we only need to reset the timerCounter, which is done below
		 * this if statement
		 */
		// Receiving bit
		Serial.print(timerCounter);
		Serial.print("\t");
		if ( abs(timerCounter - BIT_HIGH) > abs(timerCounter - BIT_LOW) ) {
			// overflowCounter is closer to VAL_HIGH
			receiveChar |= (1<<receiveCharCounter);
			Serial.print("1");
		} else {
			// overflowCounter is closer to VAL_LOW
			receiveChar &= ~(1<<receiveCharCounter);
			Serial.print("0");
		}
		Serial.print(" ");
		Serial.println(receiveCharCounter);
		receiveCharCounter++;
	} else {
		// TODO: Put this one in an if and have !=0 as the else
		// receiveCharCounter is 0 here
		
		receiveCharCounter++;
	}
	timerCounter = 0; // reset timer counter
}

void IR_initPWM(int top)
{
	TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare A
	TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler 1 and PWM, Phase and Frequency Correct
	OCR2A = PWMTOP; //the top
	OCR2B = PWMTOP / 2;
	DDRD |= (1 << DDD3); //set OC1A as output PORTB1 pin9
	TIMSK2 = (1 << TOIE2);
	sei();
}

void IR_initInterrupt() {
	PCICR |= (1<<PCIE2);	// Set pin-change interrupt for pins
	//PCIFR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT20);	// Set mask to PCINT20 (4/PD4)
	sei();
}

int main(void)
{
	Serial.begin(9600);
	IR_initPWM(PWMTOP);
	IR_initInterrupt();
	while (1)
	{
		
	}
}