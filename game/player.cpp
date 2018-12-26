#include "player.h"
#include "game.h"

Player::Player(Game *game, const char name[], uint8_t x, uint8_t y, uint16_t color, uint8_t blocksize)
    : Gameobject(x, y, false) {
    this->g = game;
    strcpy(this->name, name);
    // ongeinistaliseerd, stel de pixelposities in
    this->blocksize = blocksize;
    prevPos = {fieldPos.x * blocksize, fieldPos.y * blocksize};
    screenPos = prevPos;
    this->color = color;
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
    gfx->drawXBitmap(screenPos.x, screenPos.y,  player_still[0], color);
    gfx->drawXBitmap(screenPos.x, screenPos.y,  player_still[1], DARKBROWN);
    gfx->drawXBitmap(screenPos.x, screenPos.y,  player_still[2], SKIN);
    gfx->drawXBitmap(screenPos.x, screenPos.y,  player_still[3], YELLOW);

    // Zet opnieuw tekenen uit
    toggleRedraw(this);
}

void Player::onExplosion(Player *p) { 
    lives--; 
    g->updateScores(this);
}

void Player::giveBomb() { 
    nbombs++;
    g->updateScores(this);
}

void Player::plantBomb() {
    if (nbombs > 0) {
        g->addGameobject(new Bomb(g, fieldPos.x, fieldPos.y, this));
        nbombs--;
        g->updateScores(this);
    }
}

playerinfo Player::getPlayerinfo() {
    return {name, lives, nbombs};
}

bool Player::isMoving() { return !!this->dir; }

point Player::getScreenPos() { return this->screenPos; }

point Player::getPrevPos() { return this->prevPos; }