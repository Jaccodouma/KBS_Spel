#pragma once

#include "game.h"
#include "lib.h"

class Control {
   private:
    Game *game;

   public:
    Control();
    void update();
    void movePlayer(direction d);
    void startGame();
};