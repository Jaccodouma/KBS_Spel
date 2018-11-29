#include <avr/io.h>
#include "game.h"
#include "level.h"
#include "player.h"

// ISR(TIMER2_OVF_vect) { 
// }

int main(void) {
    init();
    Serial.begin(9600);

    // nunchuk
    DDRC |= (1 << DDC2) | (1 << DDC3);  // Set PC2 & PC3 on OUTPUT
    PORTC &= ~(1 << PORTC2);            // set PC2 to LOW
    PORTC |= (1 << PORTC3);             // Set PC3 to HIGH

    Gfx gfx;
    Control control(&gfx);
    control.startGame();

    // //timer elke 16ms
    // TCCR2A = 0x00;
    // TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20); //prescaler 1024
    // TIMSK2 |= (1<<TOIE2); // Compare match A interrupt

    while (1) {
        _delay_ms(16);
        control.update();
    }
}
