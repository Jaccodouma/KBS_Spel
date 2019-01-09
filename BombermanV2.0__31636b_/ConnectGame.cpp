#include "ConnectGame.h"

// Constructor
ConnectGame::ConnectGame(Adafruit_ILI9341 *Screen, ArduinoNunchuk *nunchuk, GameColour *gameColour, Scoreboard *scoreboard, Game *game, link *myLink, boolean master)
{
  this->Screen = Screen;
  this->nunchuk = nunchuk;
  this->gameColour = gameColour;
  this->game = game;
  this->myLink = myLink;
  this->scoreboard = scoreboard;
  this->master = master;
}

ConnectGame::run()
{
  if (newGame)
  {
    Serial.println("Drawing connect screen");
    Screen->fillScreen(this->gameColour->getGameColour());
    Screen->setTextColor(gameColour->getGameColour_negative());
    Screen->setTextSize(4);
    Screen->setCursor(13, 39);
    Screen->print("CONNECT..");
    newGame = 0;
  }

  if (game->ir->getTime_ms() > this->update_timer)
  { //do this every 500ms

    myLink->updateColorData(this->gameColour->getGameColour());
    update_timer = (game->ir->getTime_ms() + 500);
  }

  if (myLink->checkForData() > 0)
  { //keep repeating this in the loop to stay connected
    Serial.print("  color:");
    Serial.println(myLink->otherplayer_color);
    Screen->setTextColor(gameColour->getGameColour_negative());
    Screen->setTextSize(3);
    Screen->setCursor(20, 120);
    Screen->print("P2:");
    Screen->setTextColor(myLink->otherplayer_color);
    Screen->print("color");
  }

  // Check if Zbutton is pressed on nunchuck. If it is, return TASK_DONE
  nunchuk->update();
  if (nunchuk->zButton)
  {
    if (master)
    {
      Player *p = new Player(game, 1, 1, gameColour->getGameColour());
      game->addPlayer(p);
      Player *p2 = new Player(game, 13, 15, myLink->otherplayer_color);
      game->addPlayer(p2);
    }
    else
    {
      Player *p = new Player(game, 13, 15, gameColour->getGameColour());
      game->addPlayer(p);
      Player *p2 = new Player(game, 1, 1, myLink->otherplayer_color);
      game->addPlayer(p2);
    }
    Serial.println("build done");
    return TASK_DONE;
  }

  return TASK_BUSY;
}
