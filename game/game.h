#pragma once

#include "lib.h"
#include "golist.h"

#include "player.h"
#include "block.h"
#include "bomb.h"
#include "scoreboard.h"

#define REFRESHRATE 50 // elke 50ms dus 20Hz
#define MAXNPLAYER 2 // max number players

class Game {
   public:
    Player* players[MAXNPLAYER] = {NULL, NULL};

    Game(uint8_t width, uint8_t height);
    ~Game();

    bool start();
    bool isStarted();
    bool gridCollision(position p);
    Gameobject * hasCollision(Gameobject *go, position p);
    bool addPlayer(Player *p);
    void update();
    void movePlayer(Player *p, direction d);
    void bombExplosion(Bomb *bomb);
    void plantBomb(Player *p);
    void addGameobject(Gameobject *go);
    void updateScores(Player *p);
    uint8_t getWidth();
    uint8_t getHeight();

   private:
    Gfx gfx;
    GoList gos;
    uint8_t width, height;
    unsigned long prevUpdate = 0;
    bool started = false;
    Scoreboard scoreboard = Scoreboard(&gfx);

    void drawLevel();
    void addRandomBlocks();
    bool addExplosion(char x, char y, Player *p, direction dir, bool last);
    void addCornerExplosion(char x, char y, Player *p, direction dir);
    void drawGrid();
    void drawGridBlock(int x, int y);
};