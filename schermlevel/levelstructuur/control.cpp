#include "control.h"

Control::Control(View *view) { this->view = view; }

void Control::startGame() {
    game = new Game(15, 17);
    Player *p = new Player("ffk27", 1, 1, view->blockSize());
    game->addPlayer(p);
    game->start();
    view->drawLevel(game);  // teken het level-grid
    Serial.println("Game started");
}

void Control::update() {
    if (game->isStarted()) {
        movePlayer(direction::DIR_RIGHT);
    }
    game->update(&view->gfx);
}

void Control::movePlayer(direction d) {
    // laat de alleen bewegen als de speler stilstaat en de volgende positie
    // geen bostsing zou veroorzaken
    if (!game->players[0]->isMoving() &&
        !game->hasCollision(movePosition(game->players[0]->getFieldPos(), d))) {
        game->players[0]->move(d);
    }
}