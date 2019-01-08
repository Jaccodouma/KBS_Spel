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

void Control::updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives)
{
  if (x <= 15 && x > 0 && y <= 17 && y > 0 && bomb <= 1 && lives <= 3)
  { //!!!!! check if there is space in the buffer

    PlayerdataOUT[0] = (1 << 7) | ((x - 1) << 3);
    PlayerdataOUT[1] = ((y - 1) << 3) | (bomb << 2) | (lives << 0);

    PlayerdataOUT[0] |= Control::calcHash(PlayerdataOUT); //1XXX.XHHH
  }
  game->ir->write(PlayerdataOUT);
  return 0;
}

int Control::run()
{
  if (newGame)
  {
    game->start();
    newGame = 0;
    Serial.println("game started");
    return 0;
  }
  else
  {
    if (myLink->checkForData() > 0)
    { //keep repeating this in the loop to stay connected
      Serial.print(" x:");
      Serial.print(myLink->otherplayer_x);
      Serial.print("  y:");
      Serial.print(myLink->otherplayer_y);
      Serial.print("  bomb:");
      Serial.print(myLink->otherplayer_bomb);
      Serial.print("  lives:");
      Serial.print(myLink->otherplayer_lives);
      Serial.print("  color:");
      Serial.println(myLink->otherplayer_color);
      game->players[1]->plantBomb();
    }

    if (game->ir->getTime_ms() > p_update_timer)
    { //do this every 1000ms
      p_update_timer = (game->ir->getTime_ms() + 500);
      Control::updatePlayerData(3, 3, 1, 1); //X,Y,BOMB,LIVES

      Serial.println("sent data");
      return 0;
    }
    else
    {
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

uint8_t Control::calcHash(uint8_t data[MESSAGE_SIZE]) {
  uint8_t hash = 0;
  for (uint8_t bitN = 3; bitN < 8; bitN++) {
    if (data[0] & (1 << bitN)) {
      hash++;
    }
  }
  uint8_t arrayN = 1;
  while (data[arrayN] > 0) {
    for (uint8_t bitN = 0; bitN < 8; bitN++) {
      if (data[arrayN] & (1 << bitN)) {
        hash++;
      }
    }
    arrayN++;
  }
  hash = (hash % 8);
  return hash;
}
