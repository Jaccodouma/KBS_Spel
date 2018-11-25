#include <avr/io.h>
#include <stdlib.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "level.h"
#include "gfx.h"


int main(void) {
    init();
    Serial.begin(9600);

    Gfx gfx;
    Level level (gfx.gameWidth(), gfx.gameHeight());
    gfx.drawLevel(&level);
    level.printLevel();

    while(1) {
        _delay_ms(1000);
    }
}