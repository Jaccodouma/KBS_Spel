#pragma once

#include "gameobject.h"
#include <string.h>
#include "lib.h"

class Player : public Gameobject {
   public:
    Player(const char name[], uint8_t x, uint8_t y, uint8_t blocksize);
    void move(direction d);
    void update();
    void draw(Gfx *gfx);
    bool isMoving();
    position getScreenPos();
    position getPrevPos();

   private:
    char name[9];
    direction dir = direction::DIR_NO;
    //pixelposities:
    position screenPos;
    position prevPos;
    uint8_t blocksize;
};