// Auteur: Floris Folkertsma (s1094012)
#pragma once
#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "game.h"
#include "lib.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

#define BLACK ILI9341_BLACK
#define BGCOLOR ILI9341_WHITE
#define BLOCKSIZE 16

#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define SKIN 0xFEB8

class Gfx {
    public:
        Gfx();
        void drawLevel(Game *g);
        void drawPlayer1(Game *g);
        void drawPlayer2(Game *g);
        uint8_t gameWidth();
        uint8_t gameHeight();
        uint8_t blockSize();
    private:
        void drawPlayer1(uint8_t x, uint8_t y);
        void dellPlayer1(uint8_t x, uint8_t y);
        void drawPlayer2(uint8_t x, uint8_t y);
        void dellPlayer2(uint8_t x, uint8_t y);
        uint8_t offsetX, offsetY, blocksize;
        uint8_t gamewidth, gameheight;
        Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
        void drawBlock(struct block block);
};
