// Auteur: Floris Folkertsma (s1094012)
#pragma once
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Arduino.h"
#include "SPI.h"
#include "game.h"
#include "gfx.h"
#include "lib.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

class View {
   private:
    uint8_t offsetX, offsetY, blocksize;
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    void drawGrid(Game *g);
    void drawGridBlock(int x, int y);

   public:
    View();
    ~View();
    void drawLevel(Game *g);
    uint8_t blockSize();
    Gfx *gfx;
};