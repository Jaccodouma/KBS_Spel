// Auteur: Floris Folkertsma (s1094012)
#include "gfx.h"

Gfx::Gfx() {
    blocksize = BLOCKSIZE;
    tft.begin();
    tft.fillScreen(BLACK);
    tft.fillScreen(ILI9341_CYAN);

    gamewidth = tft.width() / blocksize;
    gameheight = tft.height() / blocksize;
    // maak de dimensie's van het speelveld altijd oneven
    if (isEven(gamewidth)) gamewidth--;
    if (isEven(gameheight)) gameheight--;
}

void Gfx::drawLevel(Game *g) {
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien
    // is
    offsetX = (tft.width() - g->level->getWidth() * blocksize) / 2;
    offsetY = (tft.height() - g->level->getHeight() * blocksize) / 2;
    tft.fillRect(offsetX, offsetY, tft.width() - offsetX * 2,
                  tft.height() - offsetY * 2, BGCOLOR);
    tft.drawRect(offsetX, offsetY, tft.width() - offsetX * 2,
                  tft.height() - offsetY * 2, BLACK);
    for (uint8_t i = 0; i < g->level->getHeight(); i++) {
        for (uint8_t j = 0; j < g->level->getWidth(); j++) {
            char type = g->level->level[i][j];
            if (type) {
                struct block b = { .type = type, .x = j, .y = i };
                drawBlock(b);
            }
        }
    }
}

void Gfx::drawPlayer(Game *g) {
    // teken eerst de achtergrondkleur over de vorige positie
    tft.fillCircle(g->player->getPrevPos().x + offsetX + blocksize / 2,
                    g->player->getPrevPos().y + offsetY + blocksize / 2, blocksize / 2 - 2,
                    BGCOLOR);
    tft.fillCircle(g->player->getPos().x + offsetX + blocksize / 2,
                    g->player->getPos().y + offsetY + blocksize / 2, blocksize / 2 - 2,
                    BLACK);
}
 
void Gfx::drawBlock(struct block block) {
    switch (block.type) {
        case blocktype::SOLID:
            tft.fillRect(block.x * blocksize + offsetX, block.y * blocksize + offsetY,
                          blocksize, blocksize, BLACK);
            break;
    }
}

uint8_t Gfx::gameWidth() { return this->gamewidth; }

uint8_t Gfx::gameHeight() { return this->gameheight; }

uint8_t Gfx::blockSize() { return this->blocksize; }