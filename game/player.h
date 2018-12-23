#pragma once

#include <string.h>
#include "gameobject.h"

#define NUMLIVES 3 // 3 levens als de speler start
#define PIXELSPEED 2

class Player : public Gameobject
{
  public:
    Player(const char name[], uint8_t x, uint8_t y, uint8_t blocksize=16);
    void move(direction d);
    void update(int prevUpdate);
    void draw(Gfx *gfx);
    bool isMoving();
    point getScreenPos();
    point getPrevPos();

  private:
    char name[9];
    direction dir = direction::DIR_NO;
    // pixelposities:
    point screenPos;
    point prevPos;
    uint8_t blocksize;
    uint8_t lives = NUMLIVES;
};

// de pixels van de voorkant van het poppetje:
const uint8_t player_still[5][32] PROGMEM = {
    {0b00011111, 0b11111000, 0b00010000, 0b00001000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00011111, 0b11111000, 0b00111111, 0b11111100,
     0b00001111, 0b11110000, 0b00001111, 0b11110000, 0b10100000, 0b00000101,
     0b00010010, 0b01001000},
    {0b00000000, 0b00000000, 0b00001111, 0b11110000, 0b00011000, 0b00011000,
     0b00010000, 0b00001000, 0b00010100, 0b01001000, 0b00010000, 0b00001000,
     0b00011000, 0b00011000, 0b00001111, 0b11110000, 0b00000010, 0b01000000,
     0b00111110, 0b01111100, 0b01100000, 0b00000110, 0b01000000, 0b00000010,
     0b01110000, 0b00001110, 0b00010000, 0b00001000, 0b00010011, 0b11001000,
     0b00010010, 0b01001000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11100000,
     0b00001001, 0b10010000, 0b00001001, 0b10010000, 0b00001111, 0b11110000,
     0b00000111, 0b11100000, 0b00000000, 0b00000000, 0b00000001, 0b10000000,
     0b00000001, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b01000000, 0b00000010, 0b01000000, 0b00000010,
     0b00000000, 0b00000000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001100, 0b00110000,
     0b00001100, 0b00110000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000110, 0b01100000, 0b00000010, 0b00100000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000}};

// const uint8_t player_right_walk_one[5][32] PROGMEM = {
//     {0b00000111, 0b11100000, 0b00001000, 0b00010000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000011, 0b10000000, 0b00000111, 0b10000000, 0b00000101, 0b10000000,
//      0b00000001, 0b10000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000,
//      0b00000000, 0b00000000},
//     {0b00000000, 0b00000000, 0b00000111, 0b11100000, 0b00001100, 0b00110000,
//      0b00001000, 0b00010000, 0b00001000, 0b10011000, 0b00001000, 0b00001000,
//      0b00001100, 0b00010000, 0b00000110, 0b01100000, 0b00000110, 0b01000000,
//      0b00000100, 0b01000000, 0b00001010, 0b01000000, 0b00001110, 0b01000000,
//      0b00000010, 0b01000000, 0b00000010, 0b01000000, 0b00000010, 0b10100000,
//      0b00000000, 0b00000000},
//     {
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000011,
//         0b11000000,
//         0b00000111,
//         0b00100000,
//         0b00000111,
//         0b00100000,
//         0b00000111,
//         0b11110000,
//         0b00000011,
//         0b11100000,
//         0b00000001,
//         0b10000000,
//         0b00000001,
//         0b10000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00001000,
//         0b00000000,
//         0b00001000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//     },
//     {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00000001, 0b01000000,
//      0b00000000, 0b00000000},
//     {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b11000000, 0b00000000, 0b01000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//      0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00000001, 0b01000000,
//      0b00000000, 0b00000000}};

// const uint8_t player_right_walk_two[5][32] PROGMEM = {
//     {
//         0b00000111, 0b11100000, 0b00001000, 0b00010000, 0b00000000, 0b00000000,
//         0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//         0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//         0b00000011, 0b10000000, 0b00000111, 0b10000000, 0b00000101, 0b10000000,
//         0b00000001, 0b10000000, 0b00000000, 0b00000000, 0b00001000, 0b00000000,
//         0b00000000, 0b00000000,},
//     {
//         0b00000000,
//         0b00000000,
//         0b00000111,
//         0b11100000,
//         0b00001100,
//         0b00110000,
//         0b00001000,
//         0b00010000,
//         0b00001000,
//         0b10011000,
//         0b00001000,
//         0b00001000,
//         0b00001100,
//         0b00010000,
//         0b00000110,
//         0b01100000,
//         0b00000110,
//         0b01000000,
//         0b00000100,
//         0b01000000,
//         0b00001010,
//         0b01000000,
//         0b00001110,
//         0b01000000,
//         0b00000010,
//         0b01000000,
//         0b00000010,
//         0b01000000,
//         0b00000010,
//         0b11000000,
//     },
//     {
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000011,
//         0b11000000,
//         0b00000111,
//         0b00100000,
//         0b00000111,
//         0b00100000,
//         0b00000111,
//         0b11110000,
//         0b00000011,
//         0b11100000,
//         0b00000001,
//         0b10000000,
//         0b00000001,
//         0b10000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00001000,
//         0b00000000,
//         0b00001000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//     },
//     {
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000001,
//         0b10000000,
//         0b00000001,
//         0b00000000,
//     },
//     {
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b11000000,
//         0b00000000,
//         0b01000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//     }};


//   const uint8_t player_left[5][32] PROGMEM = {  
// {
//   0b00000111, 0b11100000,
//   0b00001000, 0b00010000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000001, 0b11000000,
//   0b00000001, 0b11100000,
//   0b00000001, 0b10100000,
//   0b00000001, 0b10000000,
//   0b00000001, 0b10000000,
//   0b00000000, 0b00010000,
//   0b00000000, 0b00000000,
// },
// {
//   0b00000111, 0b11100000,
//   0b00001000, 0b00010000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000001, 0b11000000,
//   0b00000001, 0b11100000,
//   0b00000001, 0b10100000,
//   0b00000001, 0b10000000,
//   0b00000001, 0b10000000,
//   0b00000000, 0b00010000,
//   0b00000000, 0b00000000,
// },
// {
//   0b00000000, 0b00000000,
//   0b00000111, 0b11100000,
//   0b00001100, 0b00110000,
//   0b00001000, 0b00010000,
//   0b00011001, 0b00010000,
//   0b00010000, 0b00010000,
//   0b00001000, 0b00110000,
//   0b00000110, 0b01100000,
//   0b00000010, 0b01000000,
//   0b00000010, 0b00100000,
//   0b00000010, 0b00010000,
//   0b00000010, 0b01010000,
//   0b00000010, 0b01110000,
//   0b00000010, 0b01000000,
//   0b00000010, 0b01000000,
//   0b00000101, 0b01000000,
// },
// {
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000011, 0b11000000,
//   0b00000100, 0b11100000,
//   0b00000100, 0b11100000,
//   0b00001111, 0b11100000,
//   0b00000111, 0b11000000,
//   0b00000001, 0b10000000,
//   0b00000001, 0b10000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00010000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
// },
// {
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000001, 0b10000000,
//   0b00000010, 0b10000000,
// },
// {
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000011, 0b00000000,
//   0b00000010, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
//   0b00000000, 0b00000000,
// }};