#include "bomb.h"
#include "game.h"


Bomb::Bomb(Game *game, int x, int y, Player *p) : Gameobject(x, y, true) {
    this->player = p;
    this->game = game;
    toggleUpdate(this);
}

void Bomb::draw(Gfx *gfx) {
    if (state == 0) {
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[0], DARKGREY);
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[1], YELLOW);
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[2], DARKORANGE);
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[3], LIGHTGREY);
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[4], WHITE);
    }
    if (state == 1) {
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[1], RED);
        gfx->drawBitmapField(fieldPos.x, fieldPos.y, bitBomb[2], YELLOW);
    }
    toggleRedraw(this);
}

void Bomb::update(int prevUpdate) {
    state = !state;
    toggleRedraw(this);

    countdown -= prevUpdate;

    if (countdown <= 0) {
        deleteObject(this);
    }
}

void Bomb::onDelete(Gfx *gfx) {
    // teken zwart vierkant over bom
    gfx->drawRectField(fieldPos.x, fieldPos.y, CLR_BACKGROUND);
    game->bombExplosion(this);
}