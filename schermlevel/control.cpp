#include "control.h"
#include <avr/interrupt.h>



Control::Control() {
    // Nunchuk
    DDRC |= (1 << DDC2) | (1 << DDC3);  // Set PC2 & PC3 on OUTPUT
    PORTC &= ~(1 << PORTC2);            // set PC2 to LOW
    PORTC |= (1 << PORTC3);             // Set PC3 to HIGH

    this->nunchuk = ArduinoNunchuk();
    // Initialize the nunchuk
    this->nunchuk.init();


}

void Control::update() {
    Serial.println("update");
    this->nunchuk.update();
}

direction Control::nunchuck_Direction() {
    if (nunchuk.analogX >= 0 && nunchuk.analogX <= 64) {  // to the left
        return direction::DIR_LEFT;
    }

    if (nunchuk.analogX >= 192 && nunchuk.analogX <= 255) {  // to the right
        return direction::DIR_RIGHT;
    }

    if (nunchuk.analogY >= 0 && nunchuk.analogY <= 64) {  // down
        return direction::DIR_DOWN;
    }

    if (nunchuk.analogY >= 192 && nunchuk.analogY <= 255) {  // up
        return direction::DIR_UP;
    }
}