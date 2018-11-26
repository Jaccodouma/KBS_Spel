#include <avr/io.h>
#include "game.h"
#include "gfx.h"
#include "level.h"
#include "player.h"

ISR(TIMER2_OVF_vect) { 
}

int main(void) {
    init();
    Serial.begin(9600);

    Gfx gfx;
    Control control;

    Level level(gfx.gameWidth(), gfx.gameHeight());
    char name[] = "ffk27";
    Player p1(name);
    Game game(&level, &p1, &gfx);
    game.start();

    //timer elke 16ms
    TCCR2A = 0x00;
    TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20); //prescaler 1024
    TIMSK2 |= (1<<TOIE2); // Compare match A interrupt

    while (1) {
        _delay_ms(16);
        control.update();
    }
}