#pragma once
#include "gameobject.h"
class Block : public Gameobject {
    public:
        Block(uint8_t x, uint8_t y);
        void update();
        void draw(Gfx *gfx);
};