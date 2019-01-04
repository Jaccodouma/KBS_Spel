// Dit bestand bevat defines, structures en enums die op meerdere plaatsen 
// in de code gebruikt worden maar niet in de context van een object hoeven
#pragma once

#include "Arduino.h"

#define isEven(n) (n % 2 == 0)

typedef enum { DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction;

typedef struct {
    char x, y;
} position;

typedef struct {
    int x, y;
} point;

position movePosition(position p, direction d, uint8_t distance = 1);

point movePoint(point p, direction d, uint8_t distance = 1);

void printPosition(position p);

void freeRam();

typedef uint8_t bitmap16_t[32];