#include <stdlib.h>
#include "control.h"
#include "lib/list.h"

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

    Control control;
    control.startGame();

    // List<Gameobject> gos;
    // Gameobject go = new Gameobject(1, 2);
    // gos.add(go);

    while(1) {
        _delay_ms(1000);
        //freeRam();
        control.update();
    }
}