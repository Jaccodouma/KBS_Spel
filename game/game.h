#pragma once

#include "lib.h"
#include "golist.h"

#include "player.h"
#include "bomb.h"
#include "scoreboard.h"
#include "Adafruit_ILI9341.h"

#define REFRESHRATE 50 // elke 50ms dus 20Hz
#define MAXNPLAYER 2 // max number players
#define MAXNBLOCKS 60

class Game {
   public:
    Player* players[MAXNPLAYER] = {NULL, NULL};
    uint8_t blocks[MAXNBLOCKS];

    Game(uint8_t width, uint8_t height, Adafruit_ILI9341 *tft, Gfx *gfx, Scoreboard *scoreboard);
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
    Adafruit_ILI9341 *tft;
    Gfx *gfx;
    Scoreboard *scoreboard;
    GoList gos;
    uint8_t width, height;
    unsigned long prevUpdate = 0;
    bool started = false;

    void drawLevel();
    void addRandomBlocks();
    void drawBlocks();
    bool blockCollision(position p);
    bool addExplosion(char x, char y, Player *p, direction dir, bool last);
    void addCornerExplosion(char x, char y, Player *p, direction dir);
    void drawGrid();
    void drawBlock(uint8_t x, uint8_t y, uint16_t colorfill, uint16_t colordraw);
};
