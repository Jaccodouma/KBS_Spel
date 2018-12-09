#include "player.h"

Player::Player(const char name[], uint8_t x, uint8_t y) : Gameobject(x, y) {
    strcpy(this->name, name);
}

void Player::move(direction d) {
    this->dir = d;
    this->fieldPos = movePosition(this->fieldPos, d);
    printPosition(this->fieldPos);
}