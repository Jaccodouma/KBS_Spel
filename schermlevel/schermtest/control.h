#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "game.h"
#include "level.h"
#include "player.h"
#include "gfx.h"

// defines used in drawIndicator()
#define NUNCHUK_MAX_X  255 // Maximum X value (right side of screen)
#define NUNCHUK_MAX_Y  255 // Maximum Y value (top of screen)

#define MOVESPEED 3 

class Control {
    public:
        Control(Gfx *gfx);
        ~Control();
        void update();
        void startGame();
        static position movePosition(position p, direction d);
    private:
        ArduinoNunchuk nunchuk;
        Gfx *gfx;
        Game *game;
        direction nunchuck_Direction();
};