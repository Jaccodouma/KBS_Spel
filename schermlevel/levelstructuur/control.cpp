#include "control.h"

Control::Control(View *view) { this->view = view; }

void Control::startGame() {
    game = new Game(15, 17);
    Player *p = new Player("ffk27", 1, 1, view->blockSize());
    game->addPlayer(p);
    Player *p2 = new Player("Spiel", 13, 15, view->blockSize());
    game->addPlayer(p2);
    game->start();
    view->drawLevel(game);  // teken het level-grid
    Serial.println("Game started");
}

void Control::update() {
    if (game->isStarted()) {
        movePlayer(game->players[0], static_cast<direction>(random(5)));
        movePlayer(game->players[1], static_cast<direction>(random(5)));
    }
    game->update(view->gfx);
}

void Control::movePlayer(Player *p, direction d) {
    // laat de alleen bewegen als de speler stilstaat en de volgende positie
    // geen bostsing zou veroorzaken
    if (!p->isMoving() &&
        !game->hasCollision(p, movePosition(p->getFieldPos(), d))) {
                    printPosition(p->getFieldPos());

        p->move(d);
    }
}