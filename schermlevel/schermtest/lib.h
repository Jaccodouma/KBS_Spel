#pragma once
struct position {
    int x, y;
};

typedef enum { DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction;

struct block {
    char type;
    uint8_t x;
    uint8_t y;
};