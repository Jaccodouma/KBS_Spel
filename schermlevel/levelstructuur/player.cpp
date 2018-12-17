#include "player.h"

Player::Player(const char name[], uint8_t x, uint8_t y, uint8_t blocksize)
    : Gameobject(x, y) {
    strcpy(this->name, name);
    // stel de pixelposities in
    prevPos = {(uint8_t)(x * blocksize), (uint8_t)(y * blocksize)};
    screenPos = prevPos;
    this->blocksize = blocksize;
    toggleRedraw(this);
}

void Player::move(direction d) {
    this->dir = d;
    this->fieldPos = movePosition(this->fieldPos, d);
    toggleUpdate(this);  // flag for update
}

void Player::update() {
    if (dir != direction::DIR_NO) {
        this->prevPos = this->screenPos;
        this->screenPos = movePosition(this->screenPos, dir);
        if (this->screenPos.x % blocksize == 0 &&
            this->screenPos.y % blocksize == 0) {
            toggleUpdate(this);
            dir = direction::DIR_NO;
        }
        toggleRedraw(this);
    }
}

void Player::draw(Gfx *gfx) {
    // teken eerst de achtergrondkleur over de vorige positie
    gfx->drawCircle(prevPos.x, prevPos.y, BGCOLOR);
    gfx->drawCircle(screenPos.x, screenPos.y, BGCOLOR);
    toggleRedraw(this);
}

bool Player::isMoving() { return !!dir; }

position Player::getScreenPos() { return this->screenPos; }

position Player::getPrevPos() { return this->prevPos; }