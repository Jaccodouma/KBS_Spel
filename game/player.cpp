#include "player.h"

Player::Player(const char name[], uint8_t x, uint8_t y, uint8_t blocksize)
    : Gameobject(x, y, false) {
    strcpy(this->name, name);
    // ongeinistaliseerd, stel de pixelposities in
    this->blocksize = blocksize;
    prevPos = {fieldPos.x * blocksize, fieldPos.y * blocksize};
    screenPos = prevPos;
}

void Player::move(direction d) {
    if (d != direction::DIR_NO) {
        this->dir = d;
        this->fieldPos = movePosition(this->fieldPos, d);
        toggleUpdate(this);  // flag for update
    }
}

void Player::update(int prevUpdate) {
    if (dir != direction::DIR_NO) {
        this->prevPos = this->screenPos;  // sla vorige schermpositie op
        this->screenPos = movePoint(this->screenPos, dir, PIXELSPEED);

        if (this->screenPos.x % blocksize == 0 &&
            this->screenPos.y % blocksize == 0) {
                        // naar midden van gridpunt gelopen
            toggleUpdate(this);
            this->dir = direction::DIR_NO;
        }
        toggleRedraw(this);
    }
}

void Player::draw(Gfx *gfx) {
    // Teken een zwart vierkant over de vorige positie van het poppetje
    gfx->drawRect(prevPos.x, prevPos.y, BLACK);

    // Teken alle kleuren van het poppetje
    gfx->drawBitmap(screenPos.x, screenPos.y, player_still[0], RED);
    gfx->drawBitmap(screenPos.x, screenPos.y, player_still[1], BLACK);
    gfx->drawBitmap(screenPos.x, screenPos.y, player_still[2], SKIN);
    gfx->drawBitmap(screenPos.x, screenPos.y, player_still[3], YELLOW);
    gfx->drawBitmap(screenPos.x, screenPos.y, player_still[4], WHITE);

    // Zet opnieuw tekenen uit
    toggleRedraw(this);
}

bool Player::isMoving() { return !!this->dir; }

point Player::getScreenPos() { return this->screenPos; }

point Player::getPrevPos() { return this->prevPos; }