#pragma once

#include "gfx.h"
#include "lib.h"

#define B_UPDATE 0
#define B_DELETE 1
#define B_REDRAW 2

#define needsUpdate(go) (go->flags & (1 << B_UPDATE))
#define needsDelete(go) (go->flags & (1 << B_DELETE))
#define needsRedraw(go) (go->flags & (1 << B_REDRAW))

#define toggleUpdate(go) (go->flags ^= (1 << B_UPDATE))
#define toggleRedraw(go) (go->flags ^= (1 << B_REDRAW))
#define deleteObject(go) (go->flags |= (1 << B_DELETE))

class Gameobject {
    public:
        uint8_t flags = 0x00; // b3 = redraw, b2 = delete, b1 = update
        bool solid;

        Gameobject(uint8_t x, uint8_t y, bool solid);
        position getFieldPos();
        virtual void update(int prevUpdate) = 0; // geef tijd in ms sinds vorige update mee
        virtual void draw(Gfx *gfx) = 0;
        virtual void onDelete(Gfx *gfx);

    protected:
        position fieldPos;
        uint8_t state = 0; // voor de bitmaps

};