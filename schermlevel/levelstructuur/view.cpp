// Auteur: Floris Folkertsma (s1094012)
#include "view.h"

View::View() {
    tft.begin();
    tft.fillScreen(CLR_BACKGROUND);
}

View::~View() {
    delete this->gfx;
}

void View::drawLevel(Game *g) {
    blocksize = tft.width() / g->getWidth();
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien
    // is
    offsetX = (tft.width() - g->getWidth() * blocksize) / 2;
    offsetY = (tft.height() - g->getHeight() * blocksize) / 2;
    tft.fillRect(offsetX, offsetY, tft.width() - offsetX * 2,
                 tft.height() - offsetY * 2, CLR_BACKGROUND);
    tft.drawRect(offsetX, offsetY, tft.width() - offsetX * 2,
                 tft.height() - offsetY * 2, BLACK);
    drawGrid(g);
    this->gfx = new Gfx(&tft, &offsetX, &offsetY, &blocksize);
}

void View::drawGrid(Game *g) {
    for (int i = 0; i < g->getHeight(); i++) {
        for (int j = 0; j < g->getWidth(); j++) {
            if (i == 0 ||
                i == g->getHeight() - 1) {  // bovenste of onderste rij
                drawGridBlock(j, i);
            } else {
                if (j == 0 || j == g->getWidth() - 1) {  // rand links of rechts
                    drawGridBlock(j, i);
                } else {
                    if (isEven(i) && isEven(j)) {
                        drawGridBlock(j, i);
                    }
                }
            }
        }
    }
}

void View::drawGridBlock(int x, int y) {
    tft.fillRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
                 blocksize, ILI9341_LIGHTGREY);
    tft.drawRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
                 blocksize, BLACK);
}

uint8_t View::blockSize() { return this->blocksize; }