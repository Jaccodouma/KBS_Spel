#pragma once
#include "level.h"
#include "player.h"
#include "lib.h"

class Game {
   public:
    Game(Level *level, Player *player);
    ~Game();
    Level *level;
    Player *player;
    bool isStarted();
    bool hasCollision(position p);

   private:
    bool started;
};