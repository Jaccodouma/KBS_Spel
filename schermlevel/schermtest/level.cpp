// Auteur: Floris Folkertsma (s1094012)
#include "level.h"
#include <Arduino.h>
#include <stdlib.h>

Level::Level(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;
    createLevel();
}

Level::~Level() {
    // geef het gealloceerde geheugen vrij
    for (int i=0; i<this->height; i++) {
        free(this->level[i]);
    }
    free(this->level);
}

void Level::createLevel() {
    initLevel();
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (i == 0 || i == this->height - 1) { // bovenste of onderste rij
                this->level[i][j] = 'S';
            } else {
                if (j == 0 || j == this->width - 1) { // rand links of rechts
                    this->level[i][j] = 'S';
                } else {
                    if (isEven(i) && isEven(j)) {
                        this->level[i][j] = 'S';
                    }
                }
            }
        }
    }
}

void Level::initLevel() {
    // initaliseer een 2d-levelarray
    this->level = (char **)malloc(this->height * sizeof(char *));
    for (int i = 0; i < this->height; i++) {
        this->level[i] = (char *)calloc(this->width, sizeof(char));
    }
}

void Level::printLevel() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            Serial.print(this->level[i][j]);
        }
        Serial.println();
    }
    Serial.println("--------------");
}

uint8_t Level::getWidth() {
    return this->width;
}

uint8_t Level::getHeight() {
    return this->height;
}