#pragma once

#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "game.h"
#include "gfx.h"
#include "lib.h"
#include "player.h"

class Control {
   private:
    ArduinoNunchuk nunchuk;
    Gfx *gfx;
    Game *game;
    direction nunchuck_Direction();

   public:
    Control(Gfx *gfx);
    void update();
    void movePlayer(direction d);
    void startGame();
};