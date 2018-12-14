#include "GameColour.h"
#include <Arduino.h>

GameColour::GameColour() {
	generateNewColour();
}

void GameColour::generateNewColour() {
	
	this->gameColour = GameColour::color565(random(100, 255),random(100, 255),random(100, 255));
}

colour GameColour::getGameColour() {
	return this->gameColour;
}

void GameColour::setGameColour(colour gameColour) {
	this->gameColour = gameColour;
}

void GameColour::setGameColour(uint8_t red, uint8_t green, uint8_t blue) {
	setGameColour(color565(red, green, blue));
}

colour GameColour::color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}