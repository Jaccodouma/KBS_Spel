#include "lib.h"

position movePosition(position p, direction d, uint8_t distance) {
    switch (d) {
        case direction::DIR_LEFT:
            p.x-=distance;
            break;
        case direction::DIR_RIGHT:
            p.x+=distance;
            break;
        case direction::DIR_DOWN:
            p.y+=distance;
            break;
        case direction::DIR_UP:
            p.y-=distance;
            break;
    }
    return p;
};

point movePoint(point p, direction d, uint8_t distance) {
    switch (d) {
        case direction::DIR_LEFT:
            p.x-=distance;
            break;
        case direction::DIR_RIGHT:
            p.x+=distance;
            break;
        case direction::DIR_DOWN:
            p.y+=distance;
            break;
        case direction::DIR_UP:
            p.y-=distance;
            break;
    }
    return p;
};

void printPosition(position p) {
    Serial.print("Positie: ");
    printXY(p.x, p.y);
}

void printPoint(point p) {
    Serial.print("Punt: ");
    printXY(p.x, p.y);
}

void printXY(int x, int y) {
    Serial.print(x, DEC);
    Serial.print(", ");
    Serial.println(y, DEC);
}

void freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  Serial.print("Vrij geheugen: ");
  Serial.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval), DEC);
  Serial.println(" kB");
}
