#pragma once

#include "lib.h"
#include "gfx.h"
#include "Adafruit_ILI9341.h"  // TFT screen

#define TEXTWIDTH 16

class Player;

// aantal punten dat je krijgt wanneer je een blokje verwoest
#define POINTSBLOCKDESTROY 25

// aantal punten dat je krijgt als je de andere speler raakt met een explosie
#define POINTSEXPLOSIONHIT 200

class Scoreboard {
    public:
        Scoreboard(Gfx *gfx, Adafruit_ILI9341 *tft);
        void init(Player *players[]);
        void update(Player *players[]);
    private:
        Gfx *gfx;
	Adafruit_ILI9341 *tft;
        
};

const uint8_t hearth[][32] PROGMEM = {
    {
   0x38, 0x18, 0x7c, 0x3c, 0xc2, 0x66, 0x81, 0xc3, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0xc0, 0x06, 0x60, 0x0c, 0x30, 0x18, 0x18, 0x18, 0x0c,
   0x30, 0x06, 0x60, 0x03, 0xc0, 0x03, 0x80, 0x01 },
   {
   0x00, 0x00, 0x00, 0x00, 0x3c, 0x18, 0x62, 0x3c, 0xf2, 0x7f, 0xfe, 0x7f,
   0xfe, 0x7f, 0xf2, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xe0, 0x03,
   0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 },
   {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
