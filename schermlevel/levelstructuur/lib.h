// Dit bestand bevat defines, structures en enums die op meerdere plaatsen 
// in de code gebruikt worden maar niet in de context van een object hoeven
#pragma once

#include "Arduino.h"
#include "Adafruit_ILI9341.h"

#define isEven(n) (n % 2 == 0)

#define BLACK ILI9341_BLACK
#define BGCOLOR ILI9341_WHITE

typedef enum { DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction;

typedef struct {
    uint8_t x, y;
} position;

typedef struct {
    uint8_t *offsetX, *offsetY, *blocksize;
    Adafruit_ILI9341 *tft;
} Gfx;

position movePosition(position p, direction d);

void printPosition(position p);