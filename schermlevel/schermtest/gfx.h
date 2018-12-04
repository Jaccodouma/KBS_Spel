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
#define BLOCKSIZE 32

#define isEven(n) (n % 2 == 0)

class Gfx {
    public:
        Gfx();
        void drawLevel(Game *g);
        void drawPlayer(Game *g);
        uint8_t gameWidth();
        uint8_t gameHeight();
        uint8_t blockSize();
    private:
        uint8_t offsetX, offsetY, blocksize;
        uint8_t gamewidth, gameheight;
        Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
        void drawBlock(struct block block);
};