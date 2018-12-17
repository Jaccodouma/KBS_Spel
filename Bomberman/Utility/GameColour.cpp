#include "GameColour.h"
#include <Arduino.h>

GameColour::GameColour() {
	generateNewColour();
	
	this->backgroundColour = color565(40,40,40);
}

void GameColour::generateNewColour() {
	randomSeed(random());
	uint8_t r = random(100,255);
	uint8_t g = random(100,255);
	uint8_t b = random(100,255);
	this->gameColour = GameColour::color565(r, g, b);
	this->gameColour_050 = GameColour::color565(0.5*r, 0.5*g, 0.5*b);
	this->gameColour_negative = GameColour::color565(255-r, 255-g, 255-b);
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

colour GameColour::getGameColour() {
	return this->gameColour;
}

colour GameColour::getGameColour_050() {
	return this->gameColour_050;
}

colour GameColour::getBackgroundColour() {
	return this->backgroundColour;
}

colour GameColour::getGameColour_negative() {
	return this->gameColour_negative;
}