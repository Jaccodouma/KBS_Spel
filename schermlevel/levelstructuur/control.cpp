#include "control.h"

Control::Control() {}

void Control::startGame() {
    game = new Game(15, 15);
    Player *p = new Player("ffk27", 1, 1);
    game->addPlayer(p);
    game->start();
    //game->printField();
    Serial.println("Game started");
}

void Control::update() {
    if (game->isStarted()) {
        movePlayer(direction::DIR_RIGHT);
    }
}

void Control::movePlayer(direction d) {
    if (!game->hasCollision(
            movePosition(game->players[0]->getFieldPos(), d))) {
        game->players[0]->move(d);
    }
}