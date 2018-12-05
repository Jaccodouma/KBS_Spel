#pragma once

#define isEven(n) (n % 2 == 0)

struct position {
    int x, y;
};

typedef enum { DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction;

typedef enum { SOLID = 'S' } blocktype;

struct block {
    blocktype type;
    uint8_t x;
    uint8_t y;
};