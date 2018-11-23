#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

int counter = 0;

ISR(WDT_vect) {
  Serial.println("RESET");
}

void set_WDT(void)
{
  cli();
  wdt_enable(WDTO_250MS); //watchdog timerset to 250 MS
  WDTCSR |= (1<<WDIE) | (1<<WDE); //interupt and reset
  sei();
}

void setup() {
  Serial.begin(9600);
  set_WDT();
  Serial.println("");
  Serial.println("begin: ");
}

void loop() {
  wdt_reset();
  Serial.print("loop: ");
  Serial.println(counter);
  if(counter >= 10){
    delay(265);
  }else{
    delay(264);
  }
  counter++;
}
