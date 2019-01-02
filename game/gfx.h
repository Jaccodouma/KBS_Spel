#pragma once

#include "Adafruit_ILI9341.h"

#define CLR_BACKGROUND 0x0000
#define CLR_BACK_HI Screen.color565(150, 150, 150)
#define CLR_TEXT Screen.color565(0, 0, 0)
#define CLR_INDICATOR Screen.color565(255, 0, 255)

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define PURPLE 0x780F
#define WHITE 0xFFFF
#define SKIN 0xFEB8
#define DARKGREY 0x4A6A
#define LIGHTGREY 0xAD56
#define DARKORANGE 0xF3E2
#define LIGHTORANGE 0xEDE8
#define DARKBROWN 0x5900

// standaard tekstopmaak
#define TEXTSIZE 2
#define TEXTCOLOR WHITE

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

class Gfx {
   public:
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    uint8_t offsetX;
    uint8_t offsetY;
    uint8_t blocksize;
    
    Gfx();
    void drawRect(int x, int y, uint16_t color, bool fill = true);
    void drawRectField(int fieldX, int fieldY, uint16_t color, bool fill = true);
    void drawBitmap(int x, int y, const uint8_t *bitmap, uint16_t color);
    void drawXBitmap(int x, int y, const uint8_t *bitmap, uint16_t color);
    void drawBitmapField(int x, int y, const uint8_t *bitmap, uint16_t color);
    void drawChar(int x, int y, char c, uint8_t size = TEXTSIZE, uint16_t color = TEXTCOLOR);
    void drawText(int x, int y, char *text, uint8_t size = TEXTSIZE, uint16_t color = TEXTCOLOR);
     void drawXBitmapField(int x, int y, const uint8_t *bitmap, uint16_t color);
};