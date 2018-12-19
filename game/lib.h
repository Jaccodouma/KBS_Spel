// Dit bestand bevat defines, structures en enums die op meerdere plaatsen 
// in de code gebruikt worden maar niet in de context van een object hoeven
#pragma once

#include "Arduino.h"
#include "Adafruit_ILI9341.h"

#define isEven(n) (n % 2 == 0)

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



// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

typedef struct {
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    uint8_t offsetX;
    uint8_t offsetY;
    uint8_t blocksize;
} Gfx;

typedef enum { DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction;

typedef struct {
    int x, y;
} position;

position movePosition(position p, direction d, uint8_t distance = 1);

void printPosition(position p);