#include "player.h"
#include "game.h"

Player::Player(Game *game, uint8_t x, uint8_t y,
               uint16_t color, uint8_t blocksize)
    : Gameobject(x, y, false) {
    this->g = game;
    // ongeinistaliseerd, stel de pixelposities in
    this->blocksize = blocksize;
    prevPos = {fieldPos.x * blocksize, fieldPos.y * blocksize};
    screenPos = prevPos;
    this->color = color;
    updateGO(this);
}

void Player::move(direction d) {
    // beweeg alleen als er een richting wordt opgegeven.
    if (d != direction::DIR_NO) {
        this->dir = d;
        this->fieldPos = movePosition(this->fieldPos, d);
    }

}

void Player::update(int prevUpdate) {
    if (dir != direction::DIR_NO) { // als de speler een looprichting heeft
        this->prevPos = this->screenPos;  // sla vorige schermpositie op
        this->screenPos = movePoint(this->screenPos, dir, PIXELSPEED); // bereken de volgende pixelpunt
        if (this->screenPos.x % blocksize == 0 &&
            this->screenPos.y % blocksize == 0) {
            // naar midden van gridpunt gelopen
            this->dir = direction::DIR_NO; // zet de richting van de speler op GEEN
        }
        redraw(this); // markeer voor hertekenen
    }
    if (wasDead) { // speler was net geraakt door een exlosie 
        counter -= prevUpdate;
        if (counter <= 0) {
            wasDead = false;
        }
    }
    flags ^= (1<<TOGGLE_ONE);
}

void Player::draw(Gfx *gfx) {
    // Teken een zwart vierkant over de vorige positie van het poppetje
    gfx->drawRect(prevPos.x, prevPos.y, BLACK);

    if (wasDead) {
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[0], WHITE);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[1], WHITE);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[2], WHITE);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[3], WHITE);
    } else {
        // Teken alle kleuren van het poppetje
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[0], color);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[1], DARKBROWN);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[2], SKIN);
        gfx->drawXBitmap(screenPos.x, screenPos.y, player_still[3], YELLOW);
    }

    // Zet opnieuw tekenen uit
    disableRedraw(this);
}


void Player::onExplosion(Player *p) {
    if (wasDead) {
        return;  // onsterverlijk
    }
    lives--;
    if (lives > 0) {
        wasDead = true;
        counter = IMMORALTIME;
    } else {
        lives = 0; //herstel leven tijdelijk
        // speler dood - proces om spel stop te zetten moet worden gestart
    }
    if (p != this) { // door andere speler zijn explosie geraakt
        p->giveScore(POINTSEXPLOSIONHIT);
    }
    g->updateScores(this);
}

void Player::giveBomb() {
    nbombs++;
    g->updateScores(this);
}

void Player::plantBomb() {
    if (nbombs > 0) {
        g->addGameobject(new Bomb(g, fieldPos.x, fieldPos.y, this));
        // bom geplaatst
        nbombs--;
        g->updateScores(this);
    }
}

void Player::giveScore(int points) {
    score += points;
    g->updateScores(this);
}

bool Player::isMoving() { return !!this->dir; }

point Player::getScreenPos() { return this->screenPos; }

point Player::getPrevPos() { return this->prevPos; }