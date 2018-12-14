#include "block.h"

Block::Block(uint8_t x, uint8_t y) : Gameobject(x, y) {
    toggleRedraw(this);
}

void Block::update() {

}

void Block::draw(Gfx *gfx) {
    gfx->drawRect(fieldPos.x, fieldPos.y, PURPLE, true); // fill
    gfx->drawRect(fieldPos.x, fieldPos.y, BLACK, false); //draw
    toggleRedraw(this);
}