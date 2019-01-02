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
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[0], RED);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[1], DARKORANGE);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[2], LIGHTORANGE);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[3], YELLOW);
}

void Explosion::onDelete(Gfx *gfx) {
    // teken zwart vierkant erover
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[0], BLACK);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[1], BLACK);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[2], BLACK);
     gfx->drawXBitmap(fieldPos.x, fieldPos.y,  explosion_middle[3], BLACK);
}


void Explosion::onPlayerCollision(Player *p) {
    p->onExplosion(p);
}