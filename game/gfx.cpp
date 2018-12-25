#include "gfx.h"

void Gfx::drawRect(int x, int y, uint16_t color, bool fill) {
    if (fill) {
        tft.fillRect(x + offsetX, y + offsetY, blocksize, blocksize, color);
    } else {
        tft.drawRect(x + offsetX, y + offsetY, blocksize, blocksize, color);
    }
}

void Gfx::drawRectField(int fieldX, int fieldY, uint16_t color, bool fill) {
    drawRect(fieldX * blocksize, fieldY * blocksize, color, fill);
}

void Gfx::drawBitmap(int x, int y, const uint8_t *bitmap, uint16_t color) {
    tft.drawXBitmap(x + offsetX, y + offsetY, bitmap, 16, 16, color);
}

void Gfx::drawBitmapField(int x, int y, const uint8_t *bitmap, uint16_t color) {
    drawBitmap(x * blocksize, y * blocksize, bitmap, color);
}