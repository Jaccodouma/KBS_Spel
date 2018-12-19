#include "gameobject.h"

Gameobject::Gameobject(uint8_t x, uint8_t y, bool solid) {
    this->fieldPos.x = x;
    this->fieldPos.y = y;
    this->solid = solid;
}

position Gameobject::getFieldPos() {
    return this->fieldPos;
}