#include "block.h"

Block::Block(uint8_t x, uint8_t y) : Gameobject(x, y, true) {
    toggleRedraw(this);
}

void Block::update(int prevUpdate) {}

void Block::draw(Gfx *gfx) {
    gfx->drawRectField(fieldPos.x, fieldPos.y, PURPLE);
    gfx->drawRectField(fieldPos.x, fieldPos.y, DARKGREY, false);

    toggleRedraw(this);
}