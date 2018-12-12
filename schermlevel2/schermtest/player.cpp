#include "player.h"
#include <stdlib.h>
#include "control.h"

Player::Player(char *name) {
    this->name = name;
    // zet positie linksbovenin scherm
    this->pos.x = 16;
    this->pos.y = 16;
    this->setPos(16, 16);
    this->movement = direction::DIR_NO;
    this->fieldPos.x = 1;
    this->fieldPos.y = 1;
}

Player::~Player() {
    free(this->name);
}

position Player::getPos() {
    return this->pos;
}

position Player::getPrevPos() {
    return this->prevPos;
}

position Player::getFieldPos() {
    return this->fieldPos;
}

void Player::setPos(int x, int y) {
    // bewaar de vorige positie
    this->prevPos.x = this->pos.x;
    this->prevPos.y = this->pos.y;

    this->pos.x = x;
    this->pos.y = y;
}

void Player::move() {
    position nextPos = Control::movePosition(pos, movement);
    setPos(nextPos.x,nextPos.y);
}

direction Player::getDirection() {
    return this->movement;
}

void Player::setDirection(direction d) {
    movement = d;
    this->fieldPos = Control::movePosition(fieldPos, movement);
}