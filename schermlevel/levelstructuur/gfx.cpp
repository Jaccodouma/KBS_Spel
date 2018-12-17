#include "gfx.h"

Gfx::Gfx(Adafruit_ILI9341 *tft, uint8_t *offsetX, uint8_t *offsetY,
         uint8_t *blocksize) {
    this->tft = tft;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->blocksize = blocksize;
}

void Gfx::drawCircle(int x, int y, uint16_t color) {
    tft->fillCircle(x + *offsetX + *blocksize / 2,
                    y + *offsetY + *blocksize / 2, (*blocksize - 1) / 8,
                    color);
}

void Gfx::drawRect(int x, int y, uint16_t color, bool fill) {
    if (fill) {
        tft->fillRect(x * *blocksize + *offsetX, y * *blocksize + *offsetY,
                      *blocksize, *blocksize, color);
    } else {
        tft->drawRect(x * *blocksize + *offsetX, y * *blocksize + *offsetY,
                      *blocksize, *blocksize, color);
    }
}