// Auteur: Floris Folkertsma (s1094012)
#include "gfx.h"

Gfx::Gfx() {
    tft.begin();
    tft.fillScreen(ILI9341_CYAN);
}

void Gfx::drawLevel(Game *g) {
    blocksize = tft.width() / g->getWidth();
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien
    // is
    offsetX = (tft.width() - g->getWidth() * blocksize) / 2;
    offsetY = (tft.height() - g->getHeight() * blocksize) / 2;
    tft.fillRect(offsetX, offsetY, tft.width() - offsetX * 2,
                 tft.height() - offsetY * 2, BGCOLOR);
    tft.drawRect(offsetX, offsetY, tft.width() - offsetX * 2,
                 tft.height() - offsetY * 2, BLACK);
    drawGrid(g);
}

void Gfx::drawGrid(Game *g) {
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
        Serial.println("");
    }
}

void Gfx::drawGridBlock(int x, int y) {
    tft.fillRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
                 blocksize, ILI9341_LIGHTGREY);
    tft.drawRect(x * blocksize + offsetX, y * blocksize + offsetY, blocksize,
                 blocksize, BLACK);
}

void Gfx::drawPlayer(Game *g) {
    // teken eerst de achtergrondkleur over de vorige positie
    // tft.fillCircle(g->player->getPrevPos().x + offsetX + blocksize / 2,
    //                 g->player->getPrevPos().y + offsetY + blocksize / 2,
    //                 blocksize / 2, BGCOLOR);
    // tft.fillCircle(g->player->getPos().x + offsetX + blocksize / 2,
    //                 g->player->getPos().y + offsetY + blocksize / 2,
    //                 blocksize / 2, BLACK);
}

uint8_t Gfx::blockSize() { return this->blocksize; }