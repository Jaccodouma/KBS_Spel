#include "block.h"

Block::Block(uint8_t x, uint8_t y) : Gameobject(x, y, true) {
}

void Block::update(int prevUpdate) {}

void Block::draw(Gfx *gfx) {
    gfx->drawRectField(fieldPos.x, fieldPos.y, PURPLE);
    gfx->drawRectField(fieldPos.x, fieldPos.y, DARKGREY, false);

    disableRedraw(this);
}

void Block::onExplosion(Player *p) {
    p->giveScore(POINTSBLOCKDESTROY);
    deleteObject(this);
}

void Block::onDelete(Gfx *gfx) {
    // teken zwart vierkant over blok
    //gfx->drawRectField(fieldPos.x, fieldPos.y, CLR_BACKGROUND);
}