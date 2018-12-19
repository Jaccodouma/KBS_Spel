// Libraries for screen
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// Misc. libraries
#include <string.h>
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>

// Misc. defines
#define BAUDRATE 19200

// default defines for ADAfruit shield
#define TFT_DC 9
#define TFT_CS 10


// defines for screen
#define SCREEN_W 240 // Screen is 240 pixels wide
#define SCREEN_H 320 // Screen is 320 pixels high

#define CLR_BACKGROUND    0x0000
#define CLR_BACK_HI     Screen.color565(150,150,150)
#define CLR_TEXT      Screen.color565(  0,  0,  0)
#define CLR_INDICATOR   Screen.color565(255,  0,255)

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define SKIN 0xFEB8


#define INDICATOR_RADIUS 6 // Indicator radius (in pixels)

// defines used in drawIndicator()
#define NUNCHUK_MAX_X  255 // Maximum X value (right side of screen)
#define NUNCHUK_MAX_Y  255 // Maximum Y value (top of screen)


//defines used in nunchuckRichting()
#define DIR_NO    0
#define DIR_UP    1
#define DIR_DOWN  2
#define DIR_LEFT  3
#define DIR_RIGHT 4

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC); // create screen object

// Create nunchuk object
ArduinoNunchuk nunchuk = ArduinoNunchuk();


// Variables used with drawIndicator()
int old_x;
int old_y;

int w = map(w, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
int h = map(h , 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

//de pixels van de voorkant van het poppetje:
const uint8_t red_still[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b10100000, 0b00000101,
};

const uint8_t green_still[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b10100000, 0b00000101,
};


const uint8_t black_still[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00001111, 0b11110000,
  0b00011000, 0b00011000,
  0b00010000, 0b00001000,
  0b00010100, 0b01001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00010010, 0b01001000,
};

const uint8_t skin_still[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001001, 0b10010000,
  0b00001001, 0b10010000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow_still[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00001100, 0b00110000,
};

const uint8_t white_still[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000110, 0b01100000,
  0b00000010, 0b00100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

//de achterkant van het poppetje:

const uint8_t red_back_walk_one[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t green_back_walk_one[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t black_back_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00010000, 0b00001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00010010, 0b01111000,
};

const uint8_t skin_back_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00010000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow_back_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00001100, 0b00000000,
};

//achterkant loopje 2:

const uint8_t red_back_walk_two[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t green_back_walk_two[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t black_back_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00010000, 0b00001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00011110, 0b01001000,
};

const uint8_t skin_back_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00010000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow_back_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00000000, 0b00110000,
};
//de pixels van de zijkant van het poppetje (rechts) loopje 1:

const uint8_t red_right_walk_one[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b10000000,
  0b00000111, 0b10000000,
  0b00000101, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t green_right_walk_one[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b10000000,
  0b00000111, 0b10000000,
  0b00000101, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t black_right_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001100, 0b00110000,
  0b00001000, 0b00010000,
  0b00001000, 0b10011000,
  0b00001000, 0b00001000,
  0b00001100, 0b00010000,
  0b00000110, 0b01100000,
  0b00000110, 0b01000000,
  0b00000100, 0b01000000,
  0b00001010, 0b01000000,
  0b00001110, 0b01000000,
  0b00000010, 0b01000000,
  0b00000010, 0b01000000,
  0b00000010, 0b10100000,
};

const uint8_t skin_right_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b11000000,
  0b00000111, 0b00100000,
  0b00000111, 0b00100000,
  0b00000111, 0b11110000,
  0b00000011, 0b11100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t yellow_right_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b01000000,
};

const uint8_t white_right_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b11000000,
  0b00000000, 0b01000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

//rechts loopje 2:

const uint8_t red_right_walk_two[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b10000000,
  0b00000111, 0b10000000,
  0b00000101, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t green_right_walk_two[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b10000000,
  0b00000111, 0b10000000,
  0b00000101, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t black_right_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001100, 0b00110000,
  0b00001000, 0b00010000,
  0b00001000, 0b10011000,
  0b00001000, 0b00001000,
  0b00001100, 0b00010000,
  0b00000110, 0b01100000,
  0b00000110, 0b01000000,
  0b00000100, 0b01000000,
  0b00001010, 0b01000000,
  0b00001110, 0b01000000,
  0b00000010, 0b01000000,
  0b00000010, 0b01000000,
  0b00000010, 0b11000000,
};

const uint8_t skin_right_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b11000000,
  0b00000111, 0b00100000,
  0b00000111, 0b00100000,
  0b00000111, 0b11110000,
  0b00000011, 0b11100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001000, 0b00000000,
  0b00001000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t yellow_right_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b00000000,
};

const uint8_t white_right_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b11000000,
  0b00000000, 0b01000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

//de pixels voor de linkerkant van het poppetje, loopje 1

const uint8_t red_left_walk_one[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11100000,
  0b00000001, 0b10100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
};

const uint8_t green_left_walk_one[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11100000,
  0b00000001, 0b10100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
};
const uint8_t black_left_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001100, 0b00110000,
  0b00001000, 0b00010000,
  0b00011001, 0b00010000,
  0b00010000, 0b00010000,
  0b00001000, 0b00110000,
  0b00000110, 0b01100000,
  0b00000010, 0b01000000,
  0b00000010, 0b00100000,
  0b00000010, 0b00010000,
  0b00000010, 0b01010000,
  0b00000010, 0b01110000,
  0b00000010, 0b01000000,
  0b00000010, 0b01000000,
  0b00000101, 0b01000000,
};

const uint8_t skin_left_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b11000000,
  0b00000100, 0b11100000,
  0b00000100, 0b11100000,
  0b00001111, 0b11100000,
  0b00000111, 0b11000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t yellow_left_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000010, 0b10000000,
};

const uint8_t white_left_walk_one[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b00000000,
  0b00000010, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};


//het loopje naar links van het poppetje, loopje 2:

const uint8_t red_left_walk_two[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11100000,
  0b00000001, 0b10100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
};

const uint8_t green_left_walk_two[32] PROGMEM = {
  0b00000111, 0b11100000,
  0b00001000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11100000,
  0b00000001, 0b10100000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
};
const uint8_t black_left_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001100, 0b00110000,
  0b00001000, 0b00010000,
  0b00011001, 0b00010000,
  0b00010000, 0b00010000,
  0b00001000, 0b00110000,
  0b00000110, 0b01100000,
  0b00000010, 0b01000000,
  0b00000010, 0b00100000,
  0b00000010, 0b00010000,
  0b00000010, 0b01010000,
  0b00000010, 0b01110000,
  0b00000010, 0b01000000,
  0b00000010, 0b01000000,
  0b00000011, 0b01000000,
};

const uint8_t skin_left_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b11000000,
  0b00000100, 0b11100000,
  0b00000100, 0b11100000,
  0b00001111, 0b11100000,
  0b00000111, 0b11000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

const uint8_t yellow_left_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000000, 0b10000000,
};

const uint8_t white_left_walk_two[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000011, 0b00000000,
  0b00000010, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

// het naar voren lopen van het poppetje met voet links omhoog

const uint8_t red_walk_one_front[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t green_walk_one_front[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t black_walk_one_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00001111, 0b11110000,
  0b00011000, 0b00011000,
  0b00010000, 0b00001000,
  0b00010100, 0b01001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00010010, 0b01111000,
};

const uint8_t skin_walk_one_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001001, 0b10010000,
  0b00001001, 0b10010000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow_walk_one_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00001100, 0b00000000,
};

const uint8_t white_walk_one_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000110, 0b01100000,
  0b00000010, 0b00100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

//lopen naar voren met voet rechts omhoog:

const uint8_t red_walk_two_front[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t green_walk_two_front[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000000, 0b00000000,
  0b10100000, 0b00000101,
};

const uint8_t black_walk_two_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00001111, 0b11110000,
  0b00011000, 0b00011000,
  0b00010000, 0b00001000,
  0b00010100, 0b01001000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00000010, 0b01000000,
  0b00111110, 0b01111100,
  0b01100000, 0b00000110,
  0b01000000, 0b00000010,
  0b01110000, 0b00001110,
  0b00010000, 0b00001000,
  0b00010011, 0b11001000,
  0b00011110, 0b01001000,
};

const uint8_t skin_walk_two_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11100000,
  0b00001001, 0b10010000,
  0b00001001, 0b10010000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
  0b00000000, 0b00000000,
  0b00000001, 0b10000000,
  0b00000001, 0b10000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b00000000, 0b00000000,
};

const uint8_t yellow_walk_two_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00001100, 0b00110000,
  0b00000000, 0b00110000,
};

const uint8_t white_walk_two_front[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000110, 0b01100000,
  0b00000010, 0b00100000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
};

int main(void) {
  // SETUP
  init();
  Serial.begin(BAUDRATE);

  // Nunchuk
  DDRC |= (1 << DDC2) | (1 << DDC3); // Set PC2 & PC3 on OUTPUT
  PORTC &= ~(1 << PORTC2); // set PC2 to LOW
  PORTC |= (1 << PORTC3); // Set PC3 to HIGH


  Screen.begin(); // Start screen
  Screen.fillScreen(CLR_BACKGROUND);

  // Initialize the nunchuk
  nunchuk.init();

  // LOOP
  for (;;) {
    nunchuk.update();
//    
//    drawGreen_right_walk_one(10, 10);
//    drawRed_right_walk_one(30, 30);
//    drawRed_left_walk_one(50, 50);
//    drawGreen_left_walk_one(70, 70);
//    drawRed_still(90, 90);
//    drawGreen_still(110, 110);
//    drawGreen_back_walk_one(130, 130);
//    drawRed_back_walk_one(150, 150);
//    
//    drawRed_walk_one_front(10,170);
//    drawRed_walk_two_front(10,190);
//    drawGreen_walk_one_front(10,210);
//    drawGreen_walk_two_front(10,230);
//
//    drawRed_left_walk_one(30, 170 );
//    drawRed_left_walk_two(30, 190);
//    drawGreen_left_walk_one(30, 210);
//    drawGreen_left_walk_two(30, 230);
//    
//    drawRed_right_walk_one(50, 170);
//    drawRed_right_walk_two(50, 190);
//    drawGreen_right_walk_one(50, 210);
//    drawGreen_right_walk_two(50, 230);
//
//    drawRed_back_walk_one(70, 170);
//    drawRed_back_walk_two(70, 190);
//    drawGreen_back_walk_one(70, 210);
//    drawGreen_back_walk_two(70, 230);
//    
    //  drawIndicatorRed_right(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_right(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_left(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorRed_left(nunchuk.analogX , nunchuk.analogY);
      drawIndicatorRed_still(nunchuk.analogX , nunchuk.analogY);
      
  

      
    //  drawIndicatorGreen_front(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorRed_back(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_back(nunchuk.analogX , nunchuk.analogY);
    //
        nunchuck_Direction();
//    delay(250);
  }
}

//// Draw indicator for nunchuk position
//void drawBombtest(int x, int y) {
////  // Erase previous indicator
////  deleteBomb(old_x, old_y);
////  
//  // Set old x and y values, used to erase the indicator next time
//  old_x = x;
//  old_y = y;
//
//  // Draw puppet
//  drawBomb(x, y);
//}

//bomb

void drawBomb(int x, int y) {
Screen.fillCircle(x, y, 4, BLUE);
}

void deleteBomb(int x, int y) {
Screen.fillCircle(x, y, 4, BLACK);
}





//de voorkant van het poppetje:

void drawRed_still(int x, int y) {
  Screen.drawBitmap(x, y, red_still, 16, 16, RED);
  Screen.drawBitmap(x, y, black_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_still, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_still, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_still, 16, 16, WHITE);
}

void drawGreen_still(int x, int y) {
  Screen.drawBitmap(x, y, green_still, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_still, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_still, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_still, 16, 16, WHITE);
}

void deleteRed_still(int x, int y) {
  Screen.drawBitmap(x, y, red_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_still, 16, 16, BLACK);
}

void deleteGreen_still(int x, int y) {
  Screen.drawBitmap(x, y, green_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_still, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_still, 16, 16, BLACK);
}

//teken loopje naar voren 1:

void drawRed_walk_one_front(int x, int y) {
  Screen.drawBitmap(x, y, red_walk_one_front, 16, 16, RED);
  Screen.drawBitmap(x, y, black_walk_one_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_walk_one_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_walk_one_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_walk_one_front, 16, 16, WHITE);
}

void drawGreen_walk_one_front(int x, int y) {
  Screen.drawBitmap(x, y, green_walk_one_front, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_walk_one_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_walk_one_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_walk_one_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_walk_one_front, 16, 16, WHITE);
}

//teken loopje naar voren 2:
void drawRed_walk_two_front(int x, int y) {
  Screen.drawBitmap(x, y, red_walk_two_front, 16, 16, RED);
  Screen.drawBitmap(x, y, black_walk_two_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_walk_two_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_walk_two_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_walk_two_front, 16, 16, WHITE);
}

void drawGreen_walk_two_front(int x, int y) {
  Screen.drawBitmap(x, y, green_walk_two_front, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_walk_two_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_walk_two_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_walk_two_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_walk_two_front, 16, 16, WHITE);
}

//teken de zijkant:

void drawRed_right_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_right_walk_one, 16, 16, RED);
  Screen.drawBitmap(x, y, black_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_one, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right_walk_one, 16, 16, WHITE);
}

void drawGreen_right_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_right_walk_one, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_one, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right_walk_one, 16, 16, WHITE);
}

void deleteRed_right_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right_walk_one, 16, 16, BLACK);
}

void deleteGreen_right_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right_walk_one, 16, 16, BLACK);
}

void drawRed_right_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_right_walk_two, 16, 16, RED);
  Screen.drawBitmap(x, y, black_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_two, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right_walk_two, 16, 16, WHITE);
}

void drawGreen_right_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_right_walk_two, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_two, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right_walk_two, 16, 16, WHITE);
}

void deleteRed_right_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right_walk_two, 16, 16, BLACK);
}

void deleteGreen_right_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right_walk_two, 16, 16, BLACK);
}
//links tekenen:

void drawRed_left_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_left_walk_one, 16, 16, RED);
  Screen.drawBitmap(x, y, black_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_one, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left_walk_one, 16, 16, WHITE);
}

void drawGreen_left_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_left_walk_one, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_one, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left_walk_one, 16, 16, WHITE);
}

void deleteRed_left_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left_walk_one, 16, 16, BLACK);
}

void deleteGreen_left_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left_walk_one, 16, 16, BLACK);
}

void drawRed_left_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_left_walk_two, 16, 16, RED);
  Screen.drawBitmap(x, y, black_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_two, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left_walk_two, 16, 16, WHITE);
}

void drawGreen_left_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_left_walk_two, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_two, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left_walk_two, 16, 16, WHITE);
}

void deleteRed_left_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left_walk_two, 16, 16, BLACK);
}

void deleteGreen_left_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left_walk_two, 16, 16, BLACK);
}

//teken de achterkant:

void drawRed_back_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_back_walk_one, 16, 16, RED);
  Screen.drawBitmap(x, y, black_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, skin_back_walk_one, 16, 16, SKIN);
}

void drawGreen_back_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_back_walk_one, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_one, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, skin_back_walk_one, 16, 16, SKIN);
}

void deleteRed_back_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, red_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_back_walk_one, 16, 16, BLACK);
}

void deleteGreen_back_walk_one(int x, int y) {
  Screen.drawBitmap(x, y, green_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_one, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_back_walk_one, 16, 16, BLACK);
}

void drawRed_back_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_back_walk_two, 16, 16, RED);
  Screen.drawBitmap(x, y, black_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, skin_back_walk_two, 16, 16, SKIN);
}

void drawGreen_back_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_back_walk_two, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_two, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, skin_back_walk_two, 16, 16, SKIN);
}

void deleteRed_back_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, red_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_back_walk_two, 16, 16, BLACK);
}

void deleteGreen_back_walk_two(int x, int y) {
  Screen.drawBitmap(x, y, green_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_back_walk_two, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_back_walk_two, 16, 16, BLACK);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_still(int x, int y) {
  // Erase previous indicator
  deleteRed_still(old_x, old_y);
  
  
  if(nunchuk.zButton == 1){ //als zbutton ingedrukt wordt: doe dit
  drawBomb(old_x,old_y);
  }
  
  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;




  // Draw puppet
  drawRed_still(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_still(int x, int y) {
  // Erase previous indicator
  deleteGreen_still(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_still(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_back(int x, int y) {
  // Erase previous indicator
  deleteRed_back_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_back_walk_one(x, y);
}


// Draw indicator for nunchuk position
void drawIndicatorGreen_back(int x, int y) {
  // Erase previous indicator
  deleteGreen_back_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_back_walk_one(x, y);
}

// Draw indicator for nunchuk position right
void drawIndicatorRed_right(int x, int y) {
  // Erase previous indicator
  deleteRed_right_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_right_walk_one(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_right(int x, int y) {
  // Erase previous indicator
  deleteGreen_right_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_right_walk_one(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_left(int x, int y) {
  // Erase previous indicator
  deleteGreen_left_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_left_walk_one(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_left(int x, int y) {
  // Erase previous indicator
  deleteRed_left_walk_one(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_left_walk_one(x, y);
}



void nunchuck_Direction () {

  if (nunchuk.analogX >= 0 && nunchuk.analogX <= 64) { // to the left
    Serial.println("left");
    // return DIR_LEFT;

  }

  if (nunchuk.analogX >= 192 && nunchuk.analogX <= 255) { //to the right
    Serial.println("DIR_RIGHT");
    // return DIR_RIGHT;
  }

  if (nunchuk.analogY >= 0 && nunchuk.analogY <= 64) { // down
    Serial.println("DIR_DOWN");
    // return DIR_DOWN;
  }

  if (nunchuk.analogY >= 192 && nunchuk.analogY <= 255) { // up
    Serial.println("DIR_UPft");
    //  return DIR_UP;
  }

  else {
    return DIR_NO;
  }
}
