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

#define CLR_BACKGROUND    Screen.color565(  0,  0,  0)
#define CLR_BACK_HI     Screen.color565(150,150,150)
#define CLR_TEXT      Screen.color565(  0,  0,  0)
#define CLR_INDICATOR   Screen.color565(255,  0,255)

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
int posX = 0;
int posY = 0;

int old_X = 0;
int old_Y = 0;


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
    nunchuck_Direction();
  }
}


/*



     Screen.setCursor(0, 0);
  Screen.setTextColor("white");
  Screen.setTextSize(4);
  Screen.println("Hello World!");
*/

void nunchuck_Direction () {

  if (nunchuk.analogX >= 0 && nunchuk.analogX <= 64) { // to the left
    Screen.fillCircle(old_X, old_Y, INDICATOR_RADIUS, CLR_BACKGROUND);
    Serial.println("left");
    posX--;
    // return DIR_LEFT;
  }

  if (nunchuk.analogX >= 192 && nunchuk.analogX <= 255) { //to the right
    Screen.fillCircle(old_X, old_Y, INDICATOR_RADIUS, CLR_BACKGROUND);
    Serial.println("DIR_RIGHT");
    posX++;
    // return DIR_RIGHT;
  }

  if (nunchuk.analogY >= 0 && nunchuk.analogY <= 64) { // down
    Screen.fillCircle(old_X, old_Y, INDICATOR_RADIUS, CLR_BACKGROUND);
    Serial.println("DIR_DOWN");
    // return DIR_DOWN;
    posY++;
  }

  if (nunchuk.analogY >= 192 && nunchuk.analogY <= 255) { // up
    Screen.fillCircle(old_X, old_Y, INDICATOR_RADIUS, CLR_BACKGROUND);
    Serial.println("DIR_UPft");
    posY--;
    //  return DIR_UP;
  }

  old_X = posX;
  old_Y = posY;
  
  delay(10);
}

//void dropBomb(){
//  int zButton;
//
//  if(zButton == 1){
//   Screen.fillCircle(posX, posY, INDICATOR_RADIUS, CLR_INDICATOR);
//  }
//}