#pragma once
#include "gfx.h"
#include "level.h"
#include "player.h"
#include "control.h"

class Game {
   public:
    Game(Level *level, Player *player, Gfx *gfx);
    void movePlayer(direction d);
    void start();

   private:
    Level *level;
    Player *player;
    Gfx *gfx;
};