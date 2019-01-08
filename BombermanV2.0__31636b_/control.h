#pragma once

#include "Adafruit_ILI9341.h"  // TFT screen
#include "Adafruit_STMPE610.h" // Touch
#include <ArduinoNunchuk.h>    // Nunchuk

// Self-made Libraries and utilities
#include "TaskManager.h"
#include "GameColour.h"
#include "Game.h"
#include "IR.h"
#include "link.h"
#include "ConnectGame.h"
#include "lib.h"
#include "player.h"
#include "scoreboard.h"

class Control : public Task
{

public:
  Control(ArduinoNunchuk *nunchuk, Adafruit_ILI9341 *Screen, Gfx *gfx, Scoreboard *scoreboard, Game *game, link *myLink);
  int run();
  void movePlayer(direction d);

private:
  uint8_t PlayerdataOUT[2]; //variable to store the player data
  uint8_t calcHash(uint8_t data[MESSAGE_SIZE]);
  ArduinoNunchuk *nunchuk;
  Adafruit_ILI9341 *Screen;
  Game *game;
  Gfx *gfx;
  link *myLink;
  Scoreboard *scoreboard;
  direction nunchuck_Direction();
  void startGame();
  void updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives);
  unsigned long p_update_timer = 0;
  boolean newGame = 1;
};
