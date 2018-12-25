// Auteur: Floris Folkertsma (s1094012)
#include "view.h"

View::View() {
    gfx.tft.begin();
    gfx.tft.fillScreen(CLR_BACKGROUND);
}

void View::drawLevel(Game *g) {
    gfx.blocksize = gfx.tft.width() / g->getWidth();
    // stel de offsetX in zodat het speelveld horizontaal gecentreerd is.
    gfx.offsetX = (gfx.tft.width() - g->getWidth() * gfx.blocksize) / 2;
    // stel de offsetY in zodat het speelveld onderin het scherm eindigt.
    gfx.offsetY = (gfx.tft.height() - g->getHeight() * gfx.blocksize);
    gfx.tft.fillRect(gfx.offsetX,gfx. offsetY, gfx.tft.width() - gfx.offsetX * 2,
                 gfx.tft.height() - gfx.offsetY * 2, CLR_BACKGROUND);
    gfx.tft.drawRect(gfx.offsetX, gfx.offsetY, gfx.tft.width() - gfx.offsetX * 2,
                 gfx.tft.height() - gfx.offsetY * 2, BLACK);
    drawGrid(g);
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
    gfx.drawRectField(x, y, LIGHTGREY);
    gfx.drawRectField(x, y, DARKGREY, false);
}

uint8_t View::blockSize() { return gfx.blocksize; }