#pragma once
#include "gameobject.h"

class Block : public Gameobject {
    public:
        Block(uint8_t x, uint8_t y);
        void update(int prevUpdate);
        void draw(Gfx *gfx);
        void onExplosion(Player *p);
        void onDelete(Gfx *gfx);
};