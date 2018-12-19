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

void printPosition(position p) {
    Serial.print("Positie: ");
    Serial.print(p.x, DEC);
    Serial.print(", ");
    Serial.println(p.y, DEC);
} 