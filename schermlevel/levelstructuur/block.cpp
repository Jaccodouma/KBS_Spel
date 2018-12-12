#include "block.h"

Block::Block(uint8_t x, uint8_t y) : Gameobject(x, y) {
    toggleRedraw(this);
}

void Block::update() {

}

void Block::draw(Gfx *gfx) {
    gfx->tft->fillRect(fieldPos.x * *gfx->blocksize + *gfx->offsetX,
                       fieldPos.y * *gfx->blocksize + *gfx->offsetY,
                       *gfx->blocksize, *gfx->blocksize, ILI9341_PURPLE);
    gfx->tft->drawRect(fieldPos.x * *gfx->blocksize + *gfx->offsetX,
                       fieldPos.y * *gfx->blocksize + *gfx->offsetY,
                       *gfx->blocksize, *gfx->blocksize, BLACK);
    toggleRedraw(this);
}