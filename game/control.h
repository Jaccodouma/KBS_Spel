#pragma once

#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "game.h"
#include "lib.h"
#include "player.h"

class Control {
   private:
    ArduinoNunchuk nunchuk;
    Game *game;
    direction nunchuck_Direction();

   public:
    Control();
    ~Control();
    void update();
    void movePlayer(direction d);
    void startGame();
};