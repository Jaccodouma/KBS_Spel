#pragma once

#include "lib.h"
#include "gfx.h"

#define B_UPDATE 0
#define B_DELETE 1
#define B_REDRAW 2

#define needsUpdate(go) (go->flags & (1 << B_UPDATE))
#define needsDelete(go) (go->flags & (1 << B_DELETE))
#define needsRedraw(go) (go->flags & (1 << B_REDRAW))

#define toggleUpdate(go) (go->flags ^= (1 << B_UPDATE))
#define toggleRedraw(go) (go->flags ^= (1 << B_REDRAW))

class Gameobject {
    public:
        uint8_t flags = 0x00; // b3 = redraw, b2 = delete, b1 = update

        Gameobject(uint8_t x, uint8_t y);
        position getFieldPos();
        virtual void update() = 0;
        virtual void draw(Gfx *gfx) = 0;
    protected:
        position fieldPos;
};