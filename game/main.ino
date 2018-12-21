#include <avr/io.h>
#include "view.h"
#include <stdlib.h>
#include "control.h"
#include "IR.h"

IR myIR(56, 13); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void)
{
    init();
    myIR.IRinit(56, 13);
    
    Serial.begin(9600);
    Serial.println("Welkom\n");

    // nunchuk
    DDRC |= (1 << DDC2) | (1 << DDC3);  // Set PC2 & PC3 on OUTPUT
    PORTC &= ~(1 << PORTC2);            // set PC2 to LOW
    PORTC |= (1 << PORTC3);             // Set PC3 to HIGH

    View view;
    Control control(&view);
    control.startGame();

    unsigned long tijd;
    while(1) {
        //_delay_ms(8);
        tijd = millis();
        control.update();
        //freeRam();
        //Serial.println(millis() - tijd);
    }
}