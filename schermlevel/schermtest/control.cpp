#include "control.h"

Control::Control(Gfx *gfx) {
    // // Nunchuk // werkt hier niet!
    // DDRC |= (1 << DDC2) | (1 << DDC3);  // Set PC2 & PC3 on OUTPUT
    // PORTC &= ~(1 << PORTC2);            // set PC2 to LOW
    // PORTC |= (1 << PORTC3);             // Set PC3 to HIGH

    this->nunchuk = ArduinoNunchuk();
    // Initialize the nunchuk
    this->nunchuk.init();
    this->gfx = gfx;
}

Control::~Control() { delete this->game; }

void Control::update() {
    this->nunchuk.update();
    direction d = nunchuck_Direction();
    if (this->game && this->game->isStarted()) {  // als het spel gestart is
        if (d) {                                  // een richting gedrukt
            if (d && !this->game->player
                          ->getDirection()) {  // richting is gedetecteerd
                                               // en player loopt niet
                position nextPos =
                    movePosition(this->game->player->getFieldPos(), d);
                if (!this->game->hasCollision(nextPos)) {
                    this->game->player->setDirection(d);
                }
            }
        }
        if (this->game->player->getDirection()) {  // is in beweging
            this->game->player->move();
            position screenPos = this->game->player->getPos();
            if (screenPos.x % gfx->blockSize() == 0 && screenPos.y % gfx->blockSize() == 0) {
                // klaar met lopen
                this->game->player->setDirection(direction::DIR_NO);
            }
            this->gfx->drawPlayer(this->game);
        }
    }
}

void Control::startGame() {
    Level *level = new Level(this->gfx->gameWidth(), this->gfx->gameHeight());
    char *name = (char *)malloc(sizeof(char) * 9);  // acht karakter string
    name = strcpy(name, "ffk27");
    Player *p1 = new Player(name);
    this->game = new Game(level, p1);
    this->gfx->drawLevel(game);
    this->gfx->drawPlayer(game);
}

direction Control::nunchuck_Direction() {
    if (nunchuk.analogX >= 0 && nunchuk.analogX <= 64) {  // to the left
        return direction::DIR_LEFT;
    }

    if (nunchuk.analogX >= 192 && nunchuk.analogX <= 255) {  // to the right
        return direction::DIR_RIGHT;
    }

    if (nunchuk.analogY >= 0 && nunchuk.analogY <= 64) {  // down
        return direction::DIR_DOWN;
    }

    if (nunchuk.analogY >= 192 && nunchuk.analogY <= 255) {  // up
        return direction::DIR_UP;
    }
    return direction::DIR_NO;
}

position Control::movePosition(position p, direction d) {
    switch (d) {
        case direction::DIR_LEFT:
            p.x--;
            break;
        case direction::DIR_RIGHT:
            p.x++;
            break;
        case direction::DIR_DOWN:
            p.y++;
            break;
        case direction::DIR_UP:
            p.y--;
            break;
    }
    return p;
}