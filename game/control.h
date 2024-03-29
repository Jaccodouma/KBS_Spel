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

  public:
    Control(ArduinoNunchuk *nunchuk, Adafruit_ILI9341 *Screen, Gfx *gfx, Scoreboard *scoreboard);
    int run();
    void movePlayer(direction d);

  private:
    ArduinoNunchuk *nunchuk;
    Adafruit_ILI9341 *Screen;
    Game *game;
    Gfx *gfx;
    Scoreboard *scoreboard;
    direction nunchuck_Direction();
    void startGame();
};
