#pragma once
#include "gameobject.h"
#include "player.h"

class Bomb : public Gameobject {
    Bomb(int x, int y, Player *p);  
};