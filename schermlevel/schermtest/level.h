#pragma once
#include <Arduino.h>
typedef enum { SOLID = 'S' } blocktype;
class Level {
   public:
    char **level;
    Level(uint8_t width, uint8_t height);
    ~Level();
    void printLevel();
    uint8_t getWidth();
    uint8_t getHeight();

   private:
    uint8_t width, height;
    void createLevel();
    void initLevel();
};