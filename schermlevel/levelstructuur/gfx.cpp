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

// Draw indicator for nunchuk position
void Gfx::drawPlayerRed_still(position oldpos, position newpos) {
  // Erase previous indicator
  deleteRed_still(oldpos.x, oldpos.y);

  
  // Draw puppet
  drawRed_still(newpos.x, newpos.y);
}


void Gfx::drawRed_still(int x, int y) {
    x = x + *offsetX;
    y = y + *offsetY;

  tft->drawBitmap(x, y, red_still, *blocksize, *blocksize, RED);
  tft->drawBitmap(x, y, black_still, *blocksize, *blocksize, BLACK);
  tft->drawBitmap(x, y, skin_still, *blocksize, *blocksize, SKIN);
  tft->drawBitmap(x, y, yellow_still, *blocksize, *blocksize, YELLOW);
  tft->drawBitmap(x, y, white_still, *blocksize, *blocksize, WHITE);
}

void Gfx::deleteRed_still(int x, int y) {
        x = x + *offsetX;
    y = y + *offsetY;
  tft->drawBitmap(x, y, red_still, *blocksize, *blocksize, CLR_BACKGROUND);
  tft->drawBitmap(x, y, black_still, *blocksize, *blocksize, CLR_BACKGROUND);
  tft->drawBitmap(x, y, skin_still, *blocksize, *blocksize, CLR_BACKGROUND);
  tft->drawBitmap(x, y, yellow_still, *blocksize, *blocksize, CLR_BACKGROUND);
  tft->drawBitmap(x, y, white_still, *blocksize, *blocksize, CLR_BACKGROUND);
}