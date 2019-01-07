#pragma once

#include "gfx.h"
#include "lib.h"
#include "scoreboard.h"

class Game;
class Player;

#define B_UPDATE 4
#define B_DELETE 5
#define B_REDRAW 6
#define B_SOLID 7

#define needsUpdate(go) (go->flags & (1 << B_UPDATE))
#define needsDelete(go) (go->flags & (1 << B_DELETE))
#define needsRedraw(go) (go->flags & (1 << B_REDRAW))

#define deleteObject(go) (go->flags |= (1 << B_DELETE))

#define updateGO(go) (go->flags |= (1 << B_UPDATE))
#define disableUpdate(go) (go->flags &= ~(1 << B_UPDATE))
#define redraw(go) (go->flags |= (1 << B_REDRAW))
#define disableRedraw(go) (go->flags &= ~(1 << B_REDRAW))

#define isSolid(go) (go->flags & (1 << B_SOLID))

#define getState(go) (go->flags & 0x0F)

class Gameobject {
    public:
        // b7 = solid, b6 = redraw, b5 = delete, b4 = update
        uint8_t flags = (1<<B_REDRAW); // Herteken initieel 

        Gameobject(uint8_t x, uint8_t y, bool solid);
        position getFieldPos();
        virtual void update(int prevUpdate) = 0; // geef tijd in ms sinds vorige update mee
        virtual void draw(Gfx *gfx) = 0;
        virtual void onDelete(Gfx *gfx);
        virtual void onExplosion(Player *p);
        virtual void onPlayerCollision(Player *p);

    protected:
        position fieldPos;
};