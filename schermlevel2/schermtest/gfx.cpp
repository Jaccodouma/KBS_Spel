// Auteur: Floris Folkertsma (s1094012) & Christiaan van den Berg (s1112911)
#include "gfx.h"


const uint8_t red[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b10100000, 0b00000101,
};

const uint8_t green[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b10100000, 0b00000101,
};


const uint8_t black[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00001111, 0b11110000,
  0b00011000, 0b00011000,
  0b00010000, 0b00001000,
  0b00010100, 0b01001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00010010, 0b01001000,
};

const uint8_t skin[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001001, 0b10010000,
  0b00001001, 0b10010000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00001100, 0b00110000,
};

const uint8_t white[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000110, 0b01100000,
  0b00000010, 0b00100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

Gfx::Gfx() {
    tft.begin();
    tft.fillScreen(BLACK);

    blocksize = BLOCKSIZE;
    gamewidth = 15;
    gameheight = 17;
    blocksize = tft.width() / gamewidth;
    tft.setRotation(2);
}

void Gfx::drawLevel(Game *g) {
    // stel de offset in zodat het speelveld gecentreerd op het scherm te zien
    // is
    offsetX = (tft.width() - g->level->getWidth() * blocksize) / 2;
    offsetY = (tft.height() - g->level->getHeight() * blocksize);

    tft.fillRect(offsetX, offsetY, tft.width() - offsetX * 2,
                  tft.height() - offsetY, BGCOLOR);
    for (uint8_t i = 0; i < g->level->getHeight(); i++) {
        for (uint8_t j = 0; j < g->level->getWidth(); j++) {
            blocktype type = (blocktype)g->level->level[i][j];
            if (type) {
                struct block b = { .type = type, .x = j, .y = i };
                drawBlock(b);
            }
        }
    }
}

void Gfx::drawPlayer1(Game *g) {
    // teken eerst de achtergrondkleur over de vorige positie
    dellPlayer1(g->player->getPrevPos().x + offsetX,g->player->getPrevPos().y + offsetY);
    drawPlayer1(g->player->getPos().x + offsetX,g->player->getPos().y + offsetY);
}

void Gfx::drawPlayer2(Game *g) {
    // teken eerst de achtergrondkleur over de vorige positie
    dellPlayer2(g->player->getPrevPos().x + offsetX,g->player->getPrevPos().y + offsetY);
    drawPlayer2(g->player->getPos().x + offsetX,g->player->getPos().y + offsetY);
}
 
void Gfx::drawBlock(struct block block) {
    switch (block.type) {
        case blocktype::SOLID:
            tft.fillRect(block.x * blocksize + offsetX, block.y * blocksize + offsetY,
                          blocksize, blocksize, ILI9341_LIGHTGREY);
            tft.drawRect(block.x * blocksize + offsetX, block.y * blocksize + offsetY,
                          blocksize, blocksize, BLACK);
            break;
    }
}

void Gfx::drawPlayer1(uint8_t x, uint8_t y){
  tft.drawBitmap(x, y, red, 16, 16, RED);
  tft.drawBitmap(x, y, black, 16, 16, BLACK);
  tft.drawBitmap(x, y, skin, 16, 16, SKIN);
  tft.drawBitmap(x, y, yellow, 16, 16, YELLOW);
  tft.drawBitmap(x, y, white, 16, 16, WHITE);
}

void Gfx::dellPlayer1(uint8_t x, uint8_t y){
  tft.drawBitmap(x, y, red, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, black, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, skin, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, yellow, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, white, 16, 16, BGCOLOR);
}

void Gfx::drawPlayer2(uint8_t x, uint8_t y){
  tft.drawBitmap(x, y, red, 16, 16, BLUE);
  tft.drawBitmap(x, y, black, 16, 16, BLACK);
  tft.drawBitmap(x, y, skin, 16, 16, SKIN);
  tft.drawBitmap(x, y, yellow, 16, 16, YELLOW);
  tft.drawBitmap(x, y, white, 16, 16, WHITE);
}

void Gfx::dellPlayer2(uint8_t x, uint8_t y){
  tft.drawBitmap(x, y, red, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, black, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, skin, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, yellow, 16, 16, BGCOLOR);
  tft.drawBitmap(x, y, white, 16, 16, BGCOLOR);
}

uint8_t Gfx::gameWidth() { return this->gamewidth; }

uint8_t Gfx::gameHeight() { return this->gameheight; }

uint8_t Gfx::blockSize() { return this->blocksize; }
