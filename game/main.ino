#include <avr/io.h>
#include "view.h"
#include <stdlib.h>
#include "control.h"

int main(void)
{
    init();
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
       // Serial.println(millis() - tijd);
    }
}