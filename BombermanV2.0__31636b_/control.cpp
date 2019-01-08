#include "control.h"
#include "Adafruit_ILI9341.h"  // TFT screen
#include "IR.h"

Control::Control(ArduinoNunchuk *nunchuk, Adafruit_ILI9341 *Screen, Gfx *gfx, Scoreboard *scoreboard, Game *game) {
  this->nunchuk = nunchuk;
  this->Screen = Screen;
  this->gfx = gfx;
  this->game = game;
  this->scoreboard = scoreboard;
}

void Control::startGame() {
  Player *p = new Player(game, "ffk27", 1, 1, RED);
  game->addPlayer(p);
  Player *p2 = new Player(game, "Merel", 13, 15, GREEN);
  game->addPlayer(p2);
  game->start();
}

int Control::run() {
  if (newGame) {
    Serial.println("build game");
    Control::startGame();
    newGame = 0;
  }
  nunchuk->update();
  direction dir = nunchuck_Direction();

  if (game->isStarted()) {
    game->update();

    movePlayer(dir); //bedienen van speler 0 met nunchuck
    if (nunchuk->zButton == 1) {
      game->players[0]->plantBomb();
    }

  }
  return 0;
}

void Control::movePlayer(direction d) {
  game->movePlayer(game->players[0], d);
}

direction Control::nunchuck_Direction() {
  if (nunchuk->analogX >= 0 && nunchuk->analogX <= 64) {  // to the left
    return direction::DIR_LEFT;
  }

  if (nunchuk->analogX >= 192 && nunchuk->analogX <= 255) {  // to the right
    return direction::DIR_RIGHT;
  }

  if (nunchuk->analogY >= 0 && nunchuk->analogY <= 64) {  // down
    return direction::DIR_DOWN;
  }

  if (nunchuk->analogY >= 192 && nunchuk->analogY <= 255) {  // up
    return direction::DIR_UP;
  }
  return direction::DIR_NO;
}
