// ConnectGame.h

#ifndef _CONNECTGAME_h
#define _CONNECTGAME_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch
#include <ArduinoNunchuk.h>		// Nunchuk

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

class ConnectGame: public Task
{
  public:
    ConnectGame(Adafruit_ILI9341 *Screen, ArduinoNunchuk *nunchuk, GameColour *gameColour, Scoreboard *scoreboard, Game *game, IR *ir, link *myLink);
    int run();
  private:
    Adafruit_ILI9341 *Screen;
    ArduinoNunchuk *nunchuk;
    Game *game;
    IR *ir;
    link *myLink;
    Scoreboard *scoreboard;
    GameColour *gameColour;
    boolean newGame = 1;
    unsigned long update_timer = 0;
};

#endif
