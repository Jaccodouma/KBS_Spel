#include "gameobject.h"
#include "player.h"

Gameobject::Gameobject(uint8_t x, uint8_t y, bool solid) {
    this->fieldPos.x = x;
    this->fieldPos.y = y;
    if (solid) flags |= (1<<B_SOLID);
}

position Gameobject::getFieldPos() {
    return this->fieldPos;
}

void Gameobject::onDelete(Gfx *gfx) {}

void Gameobject::onExplosion(Player *p) {}

void Gameobject::onPlayerCollision(Player *p) {}
