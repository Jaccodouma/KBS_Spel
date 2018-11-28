// Auteur: Floris Folkertsma (s1094012)
#include "game.h"
#include <stdlib.h>

Game::Game(Level *level, Player *player) {
    this->level = level;
    this->player = player;
    this->started = true;
}

Game::~Game() {
    delete level;
    delete player;
}

bool Game::hasCollision(position p) {
    if (this->level->level[p.y][p.x]) { // hier zit een blokje
        return true; // collision
    }
    // buiten het speelveld
    if (p.x < 0 || p.y < 0 || p.x > this->level->getWidth() || p.y > this->level->getHeight()) {
        return true;
    }
    return false;
}

bool Game::isStarted() { return this->started; }