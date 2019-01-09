#pragma once

#include <string.h>
#include "gameobject.h"

class Game;

#define NUMLIVES 3    // 3 levens als de speler start
#define PIXELSPEED 2  // 2 pixels lopen per update
#define MAXNBOMBS 1   // 1 bom tegelijkertijd kan branden
#define MAXNAMELENGTH 6
#define IMMORALTIME 2000 // 2 seconden onstervelijkheid na aanraking met explosie
#define TOGGLE_ONE 1



typedef struct {
    char *name;
    uint8_t lives;
    int score;
} playerinfo;

class Player : public Gameobject {
   public:
    Player(Game *game, const char name[], uint8_t x, uint8_t y, uint16_t color,
           uint8_t blocksize = 16);
    char name[MAXNAMELENGTH + 1];

    void move(direction d);
    void update(int prevUpdate);
    void draw(Gfx *gfx);
    void onExplosion(Player *p);

    bool isMoving();
    point getScreenPos();
    point getPrevPos();
    void giveBomb();
    void plantBomb();
    void giveScore(int points);

    playerinfo getPlayerinfo();

   private:
    Game *g;
    direction dir = direction::DIR_NO;
    // pixelposities:
    point screenPos;
    point prevPos;
    uint8_t blocksize;
    int score = 0;
    uint8_t lives = NUMLIVES;
    uint8_t nbombs = MAXNBOMBS;
    uint16_t color;
    int counter = IMMORALTIME;
    bool wasDead = false; // wordt later verplaatst naar flags als er nog plaats is
};

const uint8_t player_still[][32] PROGMEM = {
{
   0xf8, 0x1f, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0xfc, 0x3f,
   0xf0, 0x0f, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00 },
{
   0x00, 0x00, 0xf0, 0x0f, 0x18, 0x18, 0x68, 0x16, 0x68, 0x16, 0x08, 0x10,
   0x18, 0x18, 0xf0, 0x0f, 0x40, 0x02, 0x7c, 0x3e, 0x06, 0x60, 0x02, 0x40,
   0x0e, 0x70, 0x08, 0x10, 0xc8, 0x13, 0x78, 0x1e },
{
   0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x90, 0x09, 0x90, 0x09, 0xf0, 0x0f,
   0xe0, 0x07, 0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x06, 0x60, 0x02, 0x40, 0x07, 0xe0 },
    {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00 },
{
   0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
   0x07, 0xe0, 0x0f, 0xf0, 0x3f, 0xfc, 0x03, 0xc0, 0x01, 0x80, 0x01, 0x80,
0x01, 0x80, 0x01, 0x80, 0x05, 0xa0, 0x80, 0x01 }};