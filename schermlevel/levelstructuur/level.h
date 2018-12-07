#pragma once

#include <stdlib.h>
#include <Arduino.h>
#include "gameobject.h"
#include "block.h"

#define isEven(n) (n % 2 == 0)

class Level {
   public:
    Level(int width, int height);
    ~Level();
    void printLevel();
   private:
    int width, height;
    Gameobject ***gameobjects;
    void createLevel();
    void initLevel();
    void createGrid();
};