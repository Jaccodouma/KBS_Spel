#pragma once

#include "gameobject.h"
#include <string.h>
#include "lib.h"

class Player : public Gameobject {
   public:
    Player(const char name[], uint8_t x, uint8_t y);
    void move(direction d);

   private:
    char name[9];
    direction dir;
    //pixelposities:
    position screenPos;
    position prevPos;
};