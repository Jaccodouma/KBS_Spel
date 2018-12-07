#include "level.h"

Level::Level(int width, int height) {
    this->width = width;
    this->height = height;
    createLevel();
}

Level::~Level() {
    // geef het gealloceerde geheugen vrij
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            delete this->gameobjects[i][j];
        }
        free(this->gameobjects[i]);
    }
    free(this->gameobjects);
}

void Level::createLevel() {
    initLevel();
    createGrid();
}

void Level::initLevel() {
    // initaliseer een 2d-levelarray
    this->gameobjects =
        (Gameobject ***)malloc(this->height * sizeof(Gameobject **));
    for (int i = 0; i < this->height; i++) {
        this->gameobjects[i] =
            (Gameobject **)calloc(this->width, sizeof(Gameobject *));
    }
}

void Level::createGrid() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (i == 0 || i == this->height - 1) {  // bovenste of onderste rij
                this->gameobjects[i][j] = new Block();
            } else {
                if (j == 0 || j == this->width - 1) {  // rand links of rechts
                    this->gameobjects[i][j] = new Block();
                } else {
                    if (isEven(i) && isEven(j)) {
                        this->gameobjects[i][j] = new Block();
                    }
                }
            }
        }
    }
}

void Level::printLevel() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (gameobjects[i][j]) {
                Serial.print("1");
            } else {
                Serial.print("0");
            }
        }
        Serial.println();
    }
}