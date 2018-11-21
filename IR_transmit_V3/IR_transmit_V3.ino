#include <avr/io.h>
#include <util/delay.h>

#define FREQ_56KHz 1
#define PWMTOP_56KHz 36 //36=56khz 58=34khz
#define PWMTOP_34KHz 58 //36=56khz 58=34khz
#define PWMTOP_RATIO 58 //36=56khz 58=34khz
#define BASE_VAL 500 //the overflow counter value

#define VAL_LOW 1
#define VAL_HIGH 2
#define VAL_LOW_PAR 3
#define VAL_HIGH_PAR 4

#define TURN_PULSEMODE_ON false

uint16_t OVF_counter = 0;   //pulse length counter
uint16_t Pulse_value = 0;   //value to set the pulse length
uint8_t TxCode[100];        //array that is used to prepare the to be transmitted message
uint8_t commandCounter = 0; //get the right pulse out of the array

ISR(TIMER2_OVF_vect) {
  if (OVF_counter > Pulse_value && Pulse_value > 0) { //
    OVF_counter = 0;
    TCCR2A ^= (1 << COM2B1);

    if (TxCode[commandCounter] > 0) {
      if (FREQ_56KHz) {
        Pulse_value = TxCode[commandCounter] * BASE_VAL * 1.65;
      } else {
        Pulse_value = TxCode[commandCounter] * BASE_VAL;
      }
      commandCounter++;
    } else {
      Pulse_value = 0;
      commandCounter = 0;
      TCCR2A &= ~(1 << COM2B1);
      Serial.println("Transmission ended!");
    }
  }
  OVF_counter++; //add 1 everytime the timer reaches the top (PWMTOP)
}

void IR_init(void)
{
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21); //set compare B
  TCCR2B |= (1 << CS21) | (1 << WGM22); //set clock prescaler to 8 and Fast PWM
  if (FREQ_56KHz) {
    OCR2A = PWMTOP_56KHz;
    if (TURN_PULSEMODE_ON) { //turn the 34 or 56 khz pwm mode ON or OFF (debug only)
      OCR2B = PWMTOP_56KHz / 2; //50% duty cycle
    } else {
      OCR2B = PWMTOP_56KHz ;    //100% duty cycle (without any pulses)
    }
  } else {
    OCR2A = PWMTOP_34KHz;
    if (TURN_PULSEMODE_ON) { //turn the 34 or 56 khz pwm mode ON or OFF (debug only)
      OCR2B = PWMTOP_34KHz / 2; //50% duty cycle
    } else {
      OCR2B = PWMTOP_34KHz;    //100% duty cycle (without any pulses)
    }
  }
  DDRD |= (1 << DDD3); //pin d3 OUTPUT
  TIMSK2 = (1 << TOIE2); //turn overflow interupt on
  TCCR2A &= ~(1 << COM2B1); //pin d3 LOW
  sei();


}

int main(void)
{
  Serial.begin(9600);
  IR_init();

  uint8_t TxMessage[6] = {'h', 'a', 'l', 'l', 'o'};
  //uint8_t TxMessage[6] = {'h'};
  transBytes(TxMessage);

  //transSingleByte('X');
  // _delay_ms(100);
  // transSingleByte('Y');
  // _delay_ms(100);
  //  transSingleByte('Z');
  // _delay_ms(100);
  //  transSingleByte('A');
  // _delay_ms(100);
  //  transSingleByte('B');
  //  _delay_ms(100);
  //  transSingleByte('C');
  // _delay_ms(100);

  while (1)
  {
    if (Serial.available() > 0) {
      transSingleByte(Serial.read());
    }
  }
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
  //**************debug**************
  uint8_t x = 0;
  while (massage[x] > 0) {
    Serial.print(massage[x]);
    Serial.print("-");
    x++;
  }
  Serial.println("ARRAY OF PULSES");
  //**************debug**************
}

void transBytes(uint8_t byteIn[10]) {    //transform the massage into a "array with pulses"
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
  printArray(TxCode);
  Pulse_value = 1; //start the transmission
}

void transSingleByte(uint8_t byteIn) {    //transform the massage into a "array with pulses"
  uint8_t bitNr = 0;
  for (uint8_t n = 8; n > 0; n--) { //set the pulse lenths in the array
    if (byteIn & (1 << (n - 1))) {
      TxCode[bitNr] = VAL_HIGH;
    } else {
      TxCode[bitNr] = VAL_LOW;
    }
    bitNr++;
  }
  if (has_even_parity(byteIn)) { //calculate the parity bit
    TxCode[bitNr] = VAL_LOW_PAR;     //write a 0 parity
  } else {
    TxCode[bitNr] = VAL_HIGH_PAR;    //write a 1 parity
  }
  printArray(TxCode);
  Pulse_value = 1; //start the transmission
}
