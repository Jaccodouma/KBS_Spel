// Auteur: Floris Folkertsma (s1094012)
#include "gfx.h"

Gfx::Gfx() {
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    tft.begin();
    this->tft = &tft;
    gamewidth = tft.width() / BLOCKSIZE;
    gameheight = tft.height() / BLOCKSIZE;
    // maak de dimensie's van het speelveld altijd oneven
    if (isEven(gamewidth)) gamewidth--;
    if (isEven(gameheight)) gameheight--;
}

void Gfx::drawLevel(Level *level) {
    
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien is
    offsetX = (tft->width() - level->getWidth() * BLOCKSIZE) / 2;
    offsetY = (tft->height() - level->getHeight() * BLOCKSIZE) / 2;
    tft->drawRect(offsetX, offsetY, tft->width() - offsetX * 2,
                 tft->height() - offsetY * 2, BLACK);
    for (int i = 0; i < level->getHeight(); i++) {
        for (int j = 0; j < level->getWidth(); j++) {
            char block = level->level[i][j];
            if (block) drawBlock(block, j, i);
        }
    }
}

void Gfx::drawBlock(char block, uint8_t x, uint8_t y) {
    switch (block) {
        case blocktype::SOLID:
            tft->fillRect(x * BLOCKSIZE + offsetX, y * BLOCKSIZE + offsetY,
                         BLOCKSIZE, BLOCKSIZE, BLACK);
            break;
    }
}

uint8_t Gfx::gameWidth() {
    return this->gamewidth;
}

uint8_t Gfx::gameHeight() {
    return this->gameheight;
}