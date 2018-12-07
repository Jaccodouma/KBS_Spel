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
const uint8_t red_front[32] PROGMEM = {
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

const uint8_t green_front[32] PROGMEM = {
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


const uint8_t black_front[32] PROGMEM = {
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

const uint8_t skin_front[32] PROGMEM = {
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

const uint8_t yellow_front[32] PROGMEM = {
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

const uint8_t white_front[32] PROGMEM = {
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

const uint8_t red_back[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
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

const uint8_t green_back[32] PROGMEM = {
  0b00011111, 0b11111000,
  0b00011111, 0b11111000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
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


const uint8_t black_back[32] PROGMEM = {
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
  0b00010010, 0b01001000,
};

const uint8_t skin_back[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
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

const uint8_t yellow_back[32] PROGMEM = {
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

const uint8_t purple_back[32] PROGMEM = {
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00010000, 0b00001000,
  0b00011000, 0b00011000,
  0b00001111, 0b11110000,
  0b00001111, 0b11110000,
  0b00000111, 0b11100000,
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

//de pixels van de zijkant van het poppetje (rechts):

const uint8_t red_right[32] PROGMEM = {
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

const uint8_t green_right[32] PROGMEM = {
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

const uint8_t black_right[32] PROGMEM = {
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

const uint8_t skin_right[32] PROGMEM = {
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

const uint8_t yellow_right[32] PROGMEM = {
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

const uint8_t white_right[32] PROGMEM = {
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
  0b00000001, 0b10000000,
  0b00000001, 0b01000000,
};

//de pixels voor de linkerkant van het poppetje

const uint8_t red_left[32] PROGMEM = {
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

const uint8_t green_left[32] PROGMEM = {
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
const uint8_t black_left[32] PROGMEM = {
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

const uint8_t skin_left[32] PROGMEM = {
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

const uint8_t yellow_left[32] PROGMEM = {
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

const uint8_t white_left[32] PROGMEM = {
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
    //  drawGreen_right(100,100);
    //  drawRed_right(150,150
    //  drawRed_left(100,100);
    //  drawGreen_left(150,150);
    //  drawRed_front(100,100);
    //  drawGreen_front(150,150);

    //  drawIndicatorRed_right(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_right(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_left(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorRed_left(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorRed_front(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_front(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorRed_back(nunchuk.analogX , nunchuk.analogY);
    //  drawIndicatorGreen_back(nunchuk.analogX , nunchuk.analogY);

    nunchuck_Direction();
    //delay(250);
  }
}

//de voorkant van het poppetje:

void drawRed_front(int x, int y) {
  Screen.drawBitmap(x, y, red_front, 16, 16, RED);
  Screen.drawBitmap(x, y, black_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_front, 16, 16, WHITE);
}

void drawGreen_front(int x, int y) {
  Screen.drawBitmap(x, y, green_front, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_front, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_front, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_front, 16, 16, WHITE);
}

void deleteRed_front(int x, int y) {
  Screen.drawBitmap(x, y, red_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_front, 16, 16, BLACK);
}

void deleteGreen_front(int x, int y) {
  Screen.drawBitmap(x, y, green_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_front, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_front, 16, 16, BLACK);
}



//teken de zijkant:

void drawRed_right(int x, int y) {
  Screen.drawBitmap(x, y, red_right, 16, 16, RED);
  Screen.drawBitmap(x, y, black_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right, 16, 16, WHITE);
}

void drawGreen_right(int x, int y) {
  Screen.drawBitmap(x, y, green_right, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_right, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_right, 16, 16, WHITE);
}

void deleteRed_right(int x, int y) {
  Screen.drawBitmap(x, y, red_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right, 16, 16, BLACK);
}

void deleteGreen_right(int x, int y) {
  Screen.drawBitmap(x, y, green_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_right, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_right, 16, 16, BLACK);
}

//links tekenen:

void drawRed_left(int x, int y) {
  Screen.drawBitmap(x, y, red_left, 16, 16, RED);
  Screen.drawBitmap(x, y, black_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left, 16, 16, WHITE);
}

void drawGreen_left(int x, int y) {
  Screen.drawBitmap(x, y, green_left, 16, 16, GREEN);
  Screen.drawBitmap(x, y, black_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left, 16, 16, SKIN);
  Screen.drawBitmap(x, y, yellow_left, 16, 16, YELLOW);
  Screen.drawBitmap(x, y, white_left, 16, 16, WHITE);
}

void deleteRed_left(int x, int y) {
  Screen.drawBitmap(x, y, red_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left, 16, 16, BLACK);
}

void deleteGreen_left(int x, int y) {
  Screen.drawBitmap(x, y, green_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, black_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, skin_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, yellow_left, 16, 16, BLACK);
  Screen.drawBitmap(x, y, white_left, 16, 16, BLACK);
}


//teken de achterkant:

void drawRed_back(int x, int y) {
  Screen.drawBitmap(100, 100, red_back, 16, 16, RED);
  Screen.drawBitmap(100, 100, black_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, yellow_back, 16, 16, YELLOW);
  Screen.drawBitmap(100, 100, skin_back, 16, 16, SKIN);
  Screen.drawBitmap(100, 100, purple_back, 16, 16, CLR_INDICATOR);
}

void drawGreen_back(int x, int y) {
  Screen.drawBitmap(100, 100, green_back, 16, 16, GREEN);
  Screen.drawBitmap(100, 100, black_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, yellow_back, 16, 16, YELLOW);
  Screen.drawBitmap(100, 100, skin_back, 16, 16, SKIN);
  Screen.drawBitmap(100, 100, purple_back, 16, 16, CLR_INDICATOR);
}

void deleteRed_back(int x, int y) {
  Screen.drawBitmap(100, 100, red_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, black_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, yellow_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, skin_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, purple_back, 16, 16, BLACK);
}

void deleteGreen_back(int x, int y) {
  Screen.drawBitmap(100, 100, green_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, black_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, yellow_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, skin_back, 16, 16, BLACK);
  Screen.drawBitmap(100, 100, purple_back, 16, 16, BLACK);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_front(int x, int y) {
  // Erase previous indicator
  deleteRed_front(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_front(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_front(int x, int y) {
  // Erase previous indicator
  deleteGreen_front(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_front(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_back(int x, int y) {
  // Erase previous indicator
  deleteRed_back(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_back(x, y);
}


// Draw indicator for nunchuk position
void drawIndicatorGreen_back(int x, int y) {
  // Erase previous indicator
  deleteGreen_back(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_back(x, y);
}

// Draw indicator for nunchuk position right
void drawIndicatorRed_right(int x, int y) {
  // Erase previous indicator
  deleteRed_right(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_right(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_right(int x, int y) {
  // Erase previous indicator
  deleteGreen_right(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_right(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorGreen_left(int x, int y) {
  // Erase previous indicator
  deleteGreen_left(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawGreen_left(x, y);
}

// Draw indicator for nunchuk position
void drawIndicatorRed_left(int x, int y) {
  // Erase previous indicator
  deleteRed_left(old_x, old_y);

  // Map the X and Y values in the right area (center of the screen)
  x = map(x, 0, NUNCHUK_MAX_X, 0, SCREEN_W);
  y = map(y, 0, NUNCHUK_MAX_Y, SCREEN_H, 0);

  // Set old x and y values, used to erase the indicator next time
  old_x = x;
  old_y = y;

  // Draw puppet
  drawRed_left(x, y);
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
