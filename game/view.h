// Auteur: Floris Folkertsma (s1094012)
#pragma once
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Arduino.h"
#include "SPI.h"
#include "game.h"
#include "lib.h"
#include "gfx.h"

class View {
   public:
    View();
    void drawLevel(Game *g);
    Gfx gfx;
    uint8_t blockSize();

   private:
    void drawGrid(Game *g);
    void drawGridBlock(int x, int y);
};