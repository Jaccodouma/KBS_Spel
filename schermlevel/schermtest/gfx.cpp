// Auteur: Floris Folkertsma (s1094012)
#include "gfx.h"

Gfx::Gfx() {
    tft.begin();
    tft.fillScreen(BLACK);
    tft.fillScreen(ILI9341_CYAN);

    gamewidth = tft.width() / BLOCKSIZE;
    gameheight = tft.height() / BLOCKSIZE;
    // maak de dimensie's van het speelveld altijd oneven
    if (isEven(gamewidth)) gamewidth--;
    if (isEven(gameheight)) gameheight--;
}

void Gfx::drawLevel(Game *g) {
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien
    // is
    offsetX = (tft.width() - g->level->getWidth() * BLOCKSIZE) / 2;
    offsetY = (tft.height() - g->level->getHeight() * BLOCKSIZE) / 2;
    tft.fillRect(offsetX, offsetY, tft.width() - offsetX * 2,
                  tft.height() - offsetY * 2, BGCOLOR);
    tft.drawRect(offsetX, offsetY, tft.width() - offsetX * 2,
                  tft.height() - offsetY * 2, BLACK);
    for (int i = 0; i < g->level->getHeight(); i++) {
        for (int j = 0; j < g->level->getWidth(); j++) {
            char type = g->level->level[i][j];
            if (type) {
                struct block b;
                b.type = type;
                b.x = j;
                b.y = i;
                drawBlock(b);
            }
        }
    }
}

void Gfx::drawPlayer(Game *g) {
    // teken eerst de achtergrondkleur over de vorige positie
    tft.fillCircle(g->player->getPrevPos().x + offsetX + BLOCKSIZE / 2,
                    g->player->getPrevPos().y + offsetY + BLOCKSIZE / 2, BLOCKSIZE / 2 - 1,
                    BGCOLOR);
    tft.fillCircle(g->player->getPos().x + offsetX + BLOCKSIZE / 2,
                    g->player->getPos().y + offsetY + BLOCKSIZE / 2, BLOCKSIZE / 2 - 1,
                    BLACK);
}
 
void Gfx::drawBlock(struct block block) {
    switch (block.type) {
        case blocktype::SOLID:
            tft.fillRect(block.x * BLOCKSIZE + offsetX, block.y * BLOCKSIZE + offsetY,
                          BLOCKSIZE, BLOCKSIZE, BLACK);
            break;
    }
}

uint8_t Gfx::gameWidth() { return this->gamewidth; }

uint8_t Gfx::gameHeight() { return this->gameheight; }