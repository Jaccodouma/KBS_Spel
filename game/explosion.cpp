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
    const bitmap16_t *bitmap = getBitmap();

    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[0], RED);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[1], DARKORANGE);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[2], LIGHTORANGE);
    gfx->drawXBitmapField(fieldPos.x, fieldPos.y,  bitmap[3], YELLOW);

    toggleRedraw(this); // getekend dus zet redraw nu uit
}

const bitmap16_t *Explosion::getBitmap() {
    switch (getState(this)) {
        case EX_MIDDLE:
            return explosion_middle;
        case EX_HORIZONTAL:
            return explosion_horizontal_line;
        case EX_VERTICAL:
            return explosion_vertical_line;
        case EX_TOP:
            return explosion_corner_top;
        case EX_BOT:
            return explosion_corner_bottom;
        case EX_LEFT:
            return explosion_corner_left;
        case EX_RIGHT:
            return explosion_corner_right;
    };
}

void Explosion::onDelete(Gfx *gfx) {
    // teken zwart vierkant erover
    gfx->drawRectField(fieldPos.x, fieldPos.y, CLR_BACKGROUND);
}

void Explosion::onPlayerCollision(Player *p) {

    p->onExplosion(p);
}