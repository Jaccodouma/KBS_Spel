#include "control.h"

Control::Control(ArduinoNunchuk *nunchuk, Adafruit_ILI9341 *Screen, Gfx *gfx, Scoreboard *scoreboard, Game *game, link *myLink)
{
  this->nunchuk = nunchuk;
  this->Screen = Screen;
  this->gfx = gfx;
  this->myLink = myLink;
  this->game = game;
  this->scoreboard = scoreboard;
}

int Control::run()
{
  if (newGame)
  {
    game->start();
    newGame = 0;
    this->p_update_timer = (game->ir->getTime_ms() + 500);
    this->g_update_timer = (game->ir->getTime_ms() + 50);
    Serial.println("game started");
    return 0;
  }
  else
  {
    if (myLink->checkForData() > 0)
    { //keep repeating this in the loop to stay connected
      Serial.print(" x:");
      Serial.print(myLink->otherplayer_x);
      Serial.print(" y:");
      Serial.print(myLink->otherplayer_y);
      Serial.print(" b:");
      Serial.print(myLink->otherplayer_bomb);
      Serial.print(" l:");
      Serial.print(myLink->otherplayer_lives);
      Serial.print(" c:");
      Serial.println(myLink->otherplayer_color);

      if(myLink->otherplayer_bomb){
        game->players[1]->plantBomb();
      }
    }

    if (game->ir->getTime_ms() > this->p_update_timer)
    { //do this every 1000ms
      this->p_update_timer = (game->ir->getTime_ms() + 500);
      //myLink->updatePlayerData(1, 1, 0, 0); //X,Y,BOMB,LIVES

      Serial.println("sent data");
      return 0;
    }

    if (game->ir->getTime_ms() > this->g_update_timer)
    {
      this->g_update_timer = (game->ir->getTime_ms() + 50);
      nunchuk->update();
      direction dir = nunchuck_Direction();

      if (game->isStarted())
      {
        game->update();

        movePlayer(dir); //bedienen van speler 0 met nunchuck
        if (nunchuk->zButton == 1)
        {
          game->players[0]->plantBomb();
        }
      }
    freeRam();
    }
  }
  return 0;
}

void Control::movePlayer(direction d)
{
  game->movePlayer(game->players[0], d);
}

direction Control::nunchuck_Direction()
{
  if (nunchuk->analogX >= 0 && nunchuk->analogX <= 64)
  { // to the left
    return direction::DIR_LEFT;
  }

  if (nunchuk->analogX >= 192 && nunchuk->analogX <= 255)
  { // to the right
    return direction::DIR_RIGHT;
  }

  if (nunchuk->analogY >= 0 && nunchuk->analogY <= 64)
  { // down
    return direction::DIR_DOWN;
  }

  if (nunchuk->analogY >= 192 && nunchuk->analogY <= 255)
  { // up
    return direction::DIR_UP;
  }
  return direction::DIR_NO;
}
