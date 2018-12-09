#pragma once

#include "lib.h"

class Gameobject {
    public:
        Gameobject(uint8_t x, uint8_t y);
        position getFieldPos();
    protected:
        position fieldPos;
};