#include "control.h"

Control::Control() {
    this->nunchuk = ArduinoNunchuk();
    // Initialize the nunchuk
    this->nunchuk.init();
}

Control::~Control() {
    delete this->game;
}

void Control::startGame() {
    game = new Game(15, 17);
    Player *p = new Player(game, "ffk27", 1, 1, RED);
    game->addPlayer(p);
    Player *p2 = new Player(game, "Merel", 13, 13, GREEN);
    game->addPlayer(p2);
    game->start();
    Serial.println("Game started");
}

void Control::update() {
    this->nunchuk.update();
    direction dir = nunchuck_Direction();
    if (game->isStarted()) {
        game->update();
        long ran = random(5);
        movePlayer(static_cast<direction>(ran));
        //game->movePlayer(game->players[1], static_cast<direction>(ran));
        if (random(100)==1) {
            game->players[0]->plantBomb();
        }
        if (random(100)==1) {
            //game->players[1]->plantBomb();
        }
        // movePlayer(game->players[0], dir);
        // movePlayer(game->players[1], dir);
    }
}

void Control::movePlayer(direction d) {
    game->movePlayer(game->players[0], d);
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