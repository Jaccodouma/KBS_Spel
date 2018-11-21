#include <avr/io.h>
#include <util/delay.h>

#define PWMTOP 36 //36=56khz 58=34khz
#define BASE_VAL 10000

#define VAL_LOW BASE_VAL
#define VAL_HIGH BASE_VAL*2
#define VAL_LOW_PAR BASE_VAL*3
#define VAL_HIGH_PAR BASE_VAL*4

#define TURN_PULSEMODE_ON false

uint16_t counter = 0;
uint16_t value = 0;

uint8_t sentByte;
uint16_t sentByte_par;
uint8_t bitcounter = 0;

// Timer2 overflow
ISR(TIMER2_OVF_vect) {

}

int main(void)
{
	Serial.begin(9600);
	IR_initPWM();

	while (1)
	{
		if (Serial.available()) {
			transByte(Serial.read());
		}
	}
}

void IR_initPWM(void)
{
	TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare A
	TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler 1 and PWM, Phase and Frequency Correct
	OCR2A = PWMTOP; //the top
	if (TURN_PULSEMODE_ON) {
		OCR2B = PWMTOP / 2;
		} else {
		OCR2B = PWMTOP ;
	}
	DDRD |= (1 << DDD3); //set OC1A as output PORTB1 pin9
	TIMSK2 = (1 << TOIE2);
	sei();
}

void sendByte(uint8_t byte) {
	sentByte = byte; 
} 

uint8_t has_even_parity(uint8_t x) {
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

void transByte(uint8_t byteIn) {
	sentByte = byteIn;
	bitcounter = 0;
	if (has_even_parity(byteIn)) {
		sentByte_par = VAL_HIGH_PAR;
		} else {
		sentByte_par = VAL_LOW_PAR;
	}
}