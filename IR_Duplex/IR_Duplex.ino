#include <avr/io.h>
#include <util/delay.h>

// Is it running at 56KHz? (no means 34 KHz)
#define FREQ_56KHz 1

// Maximum message size (in bytes)
#define MESSAGE_SIZE 25

// PWM TOP, used in PWM (generates frequency for the LED) 36=56khz 58=34khz
#define PWMTOP_56 36
#define PWMTOP_34 58
#define PWMTOP_RATIO 1.65

// Defines for sending data
#define VAL_LOW 1
#define VAL_HIGH 2
#define VAL_LOW_PAR 3
#define VAL_HIGH_PAR 4

// Defines for receiving data 
#define BIT_BASEVALUE 100

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

// Used for debugging: disables pulse mode
#define TURN_PULSEMODE_ON true

// Variables used for sending data 
uint16_t OVF_counter = 0;   //pulse length counter
uint16_t Pulse_value = 0;   //value to set the pulse length
uint8_t TxCode[(MESSAGE_SIZE * 9)+2];        //array that is used to prepare the to be transmitted message
uint8_t commandCounter = 0; //get the right pulse out of the array

// Variables used for receiving data 
uint16_t timerCounter = 0;
uint8_t  receiveCharCounter = 0;
uint8_t receiveChar = 0;
boolean receivingChar = false;

ISR(TIMER2_OVF_vect) {
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
				Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE * 1.65;
				} else {
				Pulse_value = TxCode[commandCounter] * BIT_BASEVALUE;
			}
			commandCounter++;
			} else {
			Pulse_value = 0;
			commandCounter = 0;
			TCCR2A &= ~(1 << COM2B1);
			//Serial.println("Transmission ended!");
		}
	}
	OVF_counter++; //add 1 everytime the timer reaches the top (PWMTOP)
}

// Pin interrupt, used for receiving data
ISR(PCINT2_vect) {
	// TODO: Replace with flag and call function in loop (?)
	detectBit();
}

// Vars with stolen function, delete soon (:
char receivedChars[MESSAGE_SIZE];   // an array to store the received data

boolean newData = false;
// End stolen vars

int main(void) {
	Serial.begin(9600);
	
	IR_init();
	
	//uint8_t TxMessage[6] = {'h', 'a', 'l', 'l', 'o'};
	//transBytes(TxMessage);
	
	while(1) {
		recvWithEndMarker(); 
		if (newData == true) {
			transBytes(receivedChars);
			newData = false;
		}
	}
} 


// Stolen function, delete soon (:
void recvWithEndMarker() {
	static byte ndx = 0;
	char endMarker = '\n';
	char rc;
	
	while (Serial.available() > 0 && newData == false) {
		rc = Serial.read();

		if (rc != endMarker) {
			receivedChars[ndx] = rc;
			ndx++;
			if (ndx >= MESSAGE_SIZE) {
				ndx = MESSAGE_SIZE - 1;
			}
		}
		else {
			receivedChars[ndx] = '\0'; // terminate the string
			ndx = 0;
			newData = true;
		}
	}
}

void IR_init(void)
{
	// TODO: Remove TURN_PULSEMODE_ON bit
	
	// PWM 
	TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare B
	TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler to 8 and Fast PWM
	if (FREQ_56KHz) {
		OCR2A = PWMTOP_56;
		if (TURN_PULSEMODE_ON) { //turn the 34 or 56 khz pwm mode ON or OFF (debug only)
			OCR2B = PWMTOP_56 / 2; //50% duty cycle
			} else {
			OCR2B = PWMTOP_56 ;    //100% duty cycle (without any pulses)
		}
		} else {
		OCR2A = PWMTOP_34;
		if (TURN_PULSEMODE_ON) { //turn the 34 or 56 khz pwm mode ON or OFF (debug only)
			OCR2B = PWMTOP_34 / 2; //50% duty cycle
			} else {
			OCR2B = PWMTOP_34;    //100% duty cycle (without any pulses)
		}
	}
	DDRD |= (1 << DDD3); //pin d3 OUTPUT
	TIMSK2 = (1 << TOIE2); //turn overflow interupt on
	TCCR2A &= ~(1 << COM2B1); //pin d3 LOW
	
	// Interrupt
	PCICR |= (1<<PCIE2);	// Set pin-change interrupt for pins
	PCMSK2 |= (1<<PCINT20);	// Set mask to PCINT20 (4/PD4)
	sei();
}

uint8_t has_even_parity(uint8_t x) { //check the parity
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

void printArray(uint8_t massage[10]) {
  /**************debug**************
  uint8_t x = 0;
  while (massage[x] > 0) {
    Serial.print(massage[x]);
    Serial.print("-");
    x++;
  }
  Serial.println("ARRAY OF PULSES");
  //**************debug**************/
}

void transBytes(uint8_t byteIn[MESSAGE_SIZE]) {    //transform the massage into a "array with pulses"
  uint8_t x = 0;
  uint8_t bitNr = 0;

  TxCode[bitNr] = 4;
  bitNr++;
  TxCode[bitNr] = 5;
  bitNr++;

  while (byteIn[x] > 0) {
    for (uint8_t n = 8; n > 0; n--) { //set the pulse lenths in the array
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
  printArray(TxCode);
  Pulse_value = 1; //start the transmission
}

// Should be called in the interrupt the IR Receiver is connected to
void detectBit() {
	switch (detectBitType(timerCounter)) {
		
		 case BITTYPE_LOW : // Received bit: 0
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			//Serial.print(0);
			break;
		case BITTYPE_HIGH : // Received bit: 1
			receiveChar = receiveChar<<1; // Shift receiveChar
			receiveCharCounter++;
			receiveChar |= 1; // set LSB to 1
			//Serial.print(1);
			break; 
		case BITTYPE_LOW_PAR : // Received bit: 0 (parity)
			if (receiveCharCounter == 0) {
				Serial.println(" | Stop");
			} else {
				//Serial.print("P:0 ");
				if (!has_even_parity(receiveChar)) {
					Serial.print("PARITY ERROR");
				}
				Serial.print((char) receiveChar);
			}
			break; 
		case BITTYPE_HIGH_PAR : // Received bit: 1 (parity)
			if (receiveCharCounter == 0) {
				Serial.print("Start | ");
			} else {
				//Serial.print("P:1 ");
				if (has_even_parity(receiveChar)) {
					Serial.print("PARITY ERROR");
				}
				Serial.print((char) receiveChar);
			}
		default: // Received bit: Start
		receiveCharCounter = 0;
	}
	
	//Serial.print(" \tTimer: ");
	//Serial.println(timerCounter);
	
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
