#include "block.h"

Block::Block(uint8_t x, uint8_t y) : Gameobject(x, y) {}

void Block::draw(Gfx *gfx) {
    // tft.fillRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
    //              blocksize, ILI9341_LIGHTGREY);
    // tft.drawRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
    //              blocksize, BLACK);
}