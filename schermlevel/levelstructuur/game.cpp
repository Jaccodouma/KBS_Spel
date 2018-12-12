#include "game.h"

Game::Game(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;
}

Game::~Game() {
    if (players[0] == NULL) delete players[0];
    if (players[1] == NULL) delete players[1];
}

void Game::update(Gfx *gfx) {
    Gameobject *go = gos.getNext();
    while (go != NULL) {
        if (needsUpdate(go)) {
            go->update();
        } 
        if (needsRedraw(go)) {
            go->draw(gfx);
        }
        go = gos.getNext();
    }
}

bool Game::hasCollision(position p) {
    if (p.y <= 0 || p.y >= height - 1) {
        // botsing met bovenste of onderste blokjes of buiten het speelveld
        return true;
    }
    if (p.x <= 0 || p.x >= width - 1) {
        // botsing met blokjes aan de zijkanten van veld of buiten het speelveld
        return true;
    }
    if (isEven(p.x) && isEven(p.y)) {
        // botsing met rasterblokjes
        return true;
    }
    return false;
}

bool Game::start() {
    if (players[0] != NULL) {  // Start bij tenminste één speler
        this->started = true;
    }
    return this->started;
}

bool Game::addPlayer(Player *p) {
    if (players[0] == NULL) {
        players[0] = p;
        gos.add(p);  // voeg toe aan de gameobject-lijst
        return true;
    }
    if (players[1] == NULL) {
        players[1] = p;
        gos.add(p);  // voeg toe aan de gameobject-lijst
        return true;
    }
    return false;  // genoeg spelers
}

bool Game::isStarted() { return this->started; }

uint8_t Game::getWidth() { return this->width; }

uint8_t Game::getHeight() { return this->height; }