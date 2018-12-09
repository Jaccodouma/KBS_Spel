#include "game.h"

Game::Game(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;
}

Game::~Game() {
    if (players[0] == NULL) delete players[0];
    if (players[1] == NULL) delete players[1];
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

void Game::printField() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1) {  // bovenste of onderste rij
                Serial.print("X");
            } else {
                if (j == 0 || j == width - 1) {  // rand links of rechts
                    Serial.print("X");
                } else {
                    if (isEven(i) && isEven(j)) {
                        Serial.print("X");
                    } else {
                        Serial.print(" ");
                    }
                }
            }
        }
        Serial.println("");
    }
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
        return true;
    }
    if (players[1] == NULL) {
        players[1] = p;
        return true;
    }
    return false;
}

bool Game::isStarted() {
    return this->started;
}