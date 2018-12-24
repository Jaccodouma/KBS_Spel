#pragma once
#include "gameobject.h"
#include "player.h"

#define EXPLOSION_RANGE 3
#define FIRE_TIME 1000 // 1 seconde

class Explosion : public Gameobject {
   public:
    Player *player;
    Explosion(uint8_t x, uint8_t y, Player *p);
    void update(int prevUpdate);
    void draw(Gfx *gfx);
    void onDelete(Gfx *gfx);
    void onPlayerCollision(Player *p);

    private:
        int countdown = FIRE_TIME;
};