#pragma once

#include "Adafruit_ILI9341.h"

#define BLACK ILI9341_BLACK
#define BGCOLOR ILI9341_WHITE
#define PURPLE ILI9341_PURPLE

class Gfx {
   public:
    Gfx(Adafruit_ILI9341 *tft, uint8_t *offsetX, uint8_t *offsetY, uint8_t *blocksize);
    void drawCircle(int x, int y, uint16_t color);
    void drawRect(int x, int y, uint16_t color, bool fill);

   private:
    uint8_t *offsetX, *offsetY, *blocksize;
    Adafruit_ILI9341 *tft;
};