#include <avr/io.h>
#include "view.h"
#include <stdlib.h>
#include "control.h"
#include "golist.h"

void freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  Serial.print("Vrij geheugen: ");
  Serial.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval), DEC);
  Serial.println(" kB");
}



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

    while(1) {
        //_delay_ms(20);
        control.update();
    }
}