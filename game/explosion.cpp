#include "explosion.h"

Explosion::Explosion(uint8_t x, uint8_t y, Player *p) : Gameobject(x, y, false) {
    this->player = p;
    toggleUpdate(this);
}

void Explosion::update(int prevUpdate) {
    countdown -= prevUpdate;
    if (countdown <= 0) {
        deleteObject(this);
    }
    toggleRedraw(this);
}


void Explosion::draw(Gfx *gfx) {
    gfx->drawRectField(fieldPos.x, fieldPos.y, YELLOW);
}

void Explosion::onDelete(Gfx *gfx) {
    // teken zwart vierkant erover
    gfx->drawRectField(fieldPos.x, fieldPos.y, CLR_BACKGROUND);
}

void Explosion::onPlayerCollision(Player *p) {
    p->onExplosion(p);
}