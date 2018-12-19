#include "player.h"

Player::Player(const char name[], uint8_t x, uint8_t y, uint8_t blocksize)
    : Gameobject(x, y, false) {
    strcpy(this->name, name);
    // stel de pixelposities in
    prevPos = {x * blocksize, y * blocksize};
    screenPos = prevPos;
    this->blocksize = blocksize;
    toggleRedraw(this);
}

void Player::move(direction d) {
    if (d != direction::DIR_NO) {
        this->dir = d;
        this->fieldPos = movePosition(this->fieldPos, d);
        toggleUpdate(this);  // flag for update
    }
}

void Player::update() {
    if (dir != direction::DIR_NO) {
        this->prevPos = this->screenPos;  // sla vorige schermpositie op
        this->screenPos = movePosition(this->screenPos, dir, 2);
        if (this->screenPos.x % blocksize ==
                0 &&  // naar midden van gridpunt gelopen
            this->screenPos.y % blocksize == 0) {
            toggleUpdate(this);
            this->dir = direction::DIR_NO;
        }
        toggleRedraw(this);
    }
}

void Player::draw(Gfx *gfx) {
    erasePlayer(gfx);
    drawColors(gfx);
    toggleRedraw(this);
}

void Player::drawColors(Gfx *gfx) {
    drawBitmap(gfx, screenPos.x, screenPos.y, player_still[0], RED);
    drawBitmap(gfx, screenPos.x, screenPos.y, player_still[1], BLACK);
    drawBitmap(gfx, screenPos.x, screenPos.y, player_still[2], SKIN);
    drawBitmap(gfx, screenPos.x, screenPos.y, player_still[3], YELLOW);
    drawBitmap(gfx, screenPos.x, screenPos.y, player_still[4], WHITE);
}

void Player::erasePlayer(Gfx *gfx) {
    for (int i = 0; i < 5; i++) {
        drawBitmap(gfx, prevPos.x, prevPos.y, player_still[i], CLR_BACKGROUND);
    }
}

void Player::drawBitmap(Gfx *gfx, int x, int y, const uint8_t *bitmap,
                        uint16_t color) {
    x += gfx->offsetX;
    y += gfx->offsetY;
    gfx->tft.drawBitmap(x, y, bitmap, gfx->blocksize, gfx->blocksize, color);
}

bool Player::isMoving() { return !!this->dir; }

position Player::getScreenPos() { return this->screenPos; }

position Player::getPrevPos() { return this->prevPos; }