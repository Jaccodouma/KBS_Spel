#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>

#include "touchScreen.h"

touchScreen::touchScreen(Adafruit_STMPE610 *touch) {
  this->touch = touch;
  if (!touch->begin()) {
  Serial.println("STMPE not found!");
  while(1);
  }
}

uint8_t touchScreen::checkmenuButton() {
  touchScreen::updateTS();
  if (x > STMP_MENU_Xmin && x < STMP_MENU_Xmax && y > STMP_MENU_Ymin && y < STMP_MENU_Ymax) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t touchScreen::checkScreenB() {
  touchScreen::updateTS();
  if (x>STMP_ScreenB_Xmin && x<STMP_ScreenB_Xmax && y>STMP_ScreenB_Ymin && y<STMP_ScreenB_Ymax) {
    return map(y, STMP_ScreenB_Ymin, STMP_ScreenB_Ymax, 5, 255);
  } else {
    return 0;
  }
}

void touchScreen::updateTS() {
  if (touch->touched()) {
    while (!touch->bufferEmpty()) {
      touch->readData(&x, &y, &z);
    }
    touch->writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
  }
}
