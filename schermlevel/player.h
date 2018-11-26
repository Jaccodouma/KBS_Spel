#pragma once
#include "level.h"
struct position {
    int x, y;
};
class Player {
   public:
    Player(char *name);
    void setPos(int x, int y);
    position getPos();
    position getPrevPos();

   private:
    char *name;
    struct position pos;
    struct position prevPos; // vorige positie
};