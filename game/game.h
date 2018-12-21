#pragma once

#include "lib.h"
#include "player.h"
#include "block.h"
#include "golist.h"
#include "bomb.h"

class Game {
   public:
    Player* players[2] = {NULL, NULL};
    Game(uint8_t width, uint8_t height);
    ~Game();
    bool start();
    bool isStarted();
    bool gridCollision(position p);
    Gameobject * hasCollision(Gameobject *go, position p);
    bool addPlayer(Player *p);
    void update(Gfx *gfx);
    uint8_t getWidth();
    uint8_t getHeight();
    GoList gos;

   private:
    uint8_t width, height;
    unsigned long prevUpdate = millis();
    bool started = false;
    void addRandomBlocks();
};