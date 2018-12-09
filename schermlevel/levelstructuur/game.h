#pragma once

#include "lib.h"
#include "player.h"

class Game {
   public:
    Player* players[2] = {NULL, NULL};
    Game(uint8_t width, uint8_t height);
    ~Game();
    bool start();
    bool isStarted();
    bool hasCollision(position p);
    bool addPlayer(Player *p);
    void printField();

   private:
    int width, height;
    bool started = false;
};