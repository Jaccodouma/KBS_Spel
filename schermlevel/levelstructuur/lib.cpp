#include "lib.h"

position movePosition(position p, direction d) {
    switch (d) {
        case direction::DIR_LEFT:
            p.x--;
            break;
        case direction::DIR_RIGHT:
            p.x++;
            break;
        case direction::DIR_DOWN:
            p.y++;
            break;
        case direction::DIR_UP:
            p.y--;
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