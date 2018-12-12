#pragma once

#include <Arduino.h>
#include <ArduinoNunchuk.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "game.h"
#include "view.h"
#include "lib.h"
#include "player.h"

class Control {
   private:
    ArduinoNunchuk nunchuk;
    View *view;
    Game *game;
    direction nunchuck_Direction();

   public:
    Control(View *view);
    void update();
    void movePlayer(Player *p, direction d);
    void startGame();
};