#include "gameobject.h"

Gameobject::Gameobject(uint8_t x, uint8_t y) {
    this->fieldPos.x = x;
    this->fieldPos.y = y;
}

position Gameobject::getFieldPos() {
    return this->fieldPos;
}