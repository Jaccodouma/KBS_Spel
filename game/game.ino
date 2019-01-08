#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"  // TFT screen
#include "Adafruit_STMPE610.h"  // Touch screen
#include <avr/io.h>
#include <stdlib.h>
#include "control.h"
#include "IR.h"

Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC); 
Adafruit_STMPE610 Touch = Adafruit_STMPE610(8);

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

        Serial.begin(9600);
    Serial.println("Welkom\n");
    
    myIR.IRinit(56, 13);
    ArduinoNunchuk nunchuk;
    nunchuk.init();
    Screen.begin();
    Gfx gfx(&Screen);
    Scoreboard scoreboard = Scoreboard(&gfx, &Screen);

    Control control(&nunchuk, &Screen, &gfx, &scoreboard);
  

    while(1) {
        control.run();
    }
}
