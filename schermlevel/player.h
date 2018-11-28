#pragma once
#include "level.h"
#include "lib.h"

class Player {
   public:
    Player(char *name);
    ~Player();
    position getPos();
    position getPrevPos();
    position getFieldPos();
    direction getDirection();
    void move();
    void setDirection(direction d);

   private:
    char *name;
    struct position pos; // coordinaten in pixels
    struct position prevPos; // vorige positie
    struct position fieldPos; // positie in raster
    direction movement;
    void setPos(int x, int y);
};