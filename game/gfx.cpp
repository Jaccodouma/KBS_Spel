#include "gfx.h"

Gfx::Gfx() {
    tft.begin();
    tft.fillScreen(CLR_BACKGROUND);
}

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
    tft.drawBitmap(x + offsetX, y + offsetY, bitmap, 16, 16, color);
}

void Gfx::drawXBitmap(int x, int y, const uint8_t *bitmap, uint16_t color) {
    tft.drawXBitmap(x + offsetX, y + offsetY, bitmap, 16, 16, color);
}

void Gfx::drawBitmapField(int x, int y, const uint8_t *bitmap, uint16_t color) {
    drawBitmap(x * blocksize, y * blocksize, bitmap, color);
}

void Gfx::drawChar(int x, int y, char c, uint8_t size, uint16_t color) {
    char str[] = {c, '\0'};
    drawText(x, y, str, size, color);
}

void Gfx::drawText(int x, int y, char *text, uint8_t size, uint16_t color) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}