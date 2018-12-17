#include "game.h"

Game::Game(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;
    addRandomBlocks();
}

Game::~Game() {
    if (players[0] == NULL) delete players[0];
    if (players[1] == NULL) delete players[1];
}

void Game::addRandomBlocks() {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            // ga langs alle posities maar sla grid-blokjes over
            if (!isEven(i) || !isEven(j)) {
                // kans is 1 op 3 dat er een blokje geplaatst wordt
                if (random(3) == 0) gos.add(new Block(j, i));
            }
        }
    }
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

bool Game::gridCollision(position p) {
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

Gameobject * Game::hasCollision(Gameobject *go, position p) {
    Gameobject *temp = gos.getNext();
    Gameobject *collision = NULL;
    while (temp != NULL) {
        position pos = temp->getFieldPos();
        if (p.x == pos.x && p.y == pos.y) {  // botsing met een game-object
            collision = temp;
        }
        temp = gos.getNext();
    }
    return collision;
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