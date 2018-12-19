#include <avr/io.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_GFX.h>
#include "touchScreen.h"
#include "settingMenu.h"

#define TFT_CS 10
#define TFT_DC 9

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 touch = Adafruit_STMPE610(8);

int main(void)
{
  init();
  touchScreen myTS(&tft, &touch);
  settingMenu myMenu(&myTS);
  
  Serial.begin(9600);
  Serial.println("hoi");
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLUE);

  while (1)
  {
    if(myMenu.run()){
     Serial.println("Ennn doooor..");
    }
   myMenu.screenBrightness();
  }
}
