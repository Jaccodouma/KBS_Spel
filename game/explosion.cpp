#include "explosion.h"

Explosion::Explosion(uint8_t x, uint8_t y, Player *p, uint8_t bitmaptype) : Gameobject(x, y, false) {
    this->player = p;
    toggleUpdate(this); // update dit object continu
    flags |= bitmaptype;
}

void Explosion::update(int prevUpdate) {
    countdown -= prevUpdate;
    if (countdown <= 0) {
        deleteObject(this);
    }
}

void Explosion::draw(Gfx *gfx) {

    // if(){

    // }
    const uint8_t bitmap[][32] = explosion_middle;
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[0], RED);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[1], DARKORANGE);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[2], LIGHTORANGE);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[3], YELLOW);
    toggleRedraw(this); // getekend dus zet redraw nu uit
}




void Explosion::onDelete(Gfx *gfx) {
    // teken zwart vierkant erover
    gfx->drawRectField(fieldPos.x, fieldPos.y, CLR_BACKGROUND);
}

void Explosion::onPlayerCollision(Player *p) {

    p->onExplosion(p);
}