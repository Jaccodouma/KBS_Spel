#include "player.h"

Player::Player(char *name) {
    this->name = name;
    this->pos.x = 0;
    this->pos.y = 0;
    this->setPos(0, 0);
}

position Player::getPos() {
    return this->pos;
}

position Player::getPrevPos() {
    return this->prevPos;
}

void Player::setPos(int x, int y) {
    // bewaar de vorige positie
    this->prevPos.x = this->pos.x;
    this->prevPos.y = this->pos.y;

    this->pos.x = x;
    this->pos.y = y;
}