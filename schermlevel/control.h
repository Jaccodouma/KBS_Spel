#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>

// defines used in drawIndicator()
#define NUNCHUK_MAX_X  255 // Maximum X value (right side of screen)
#define NUNCHUK_MAX_Y  255 // Maximum Y value (top of screen)

typedef enum {
    DIR_NO, DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} direction;

class Control {
    public:
        Control();
        direction nunchuck_Direction();
        void update();
    private:
        ArduinoNunchuk nunchuk;
};