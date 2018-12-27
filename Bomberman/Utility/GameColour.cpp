#include "GameColour.h"
#include <Arduino.h>

GameColour::GameColour() {
	generateNewColour();
	this->backgroundColour = color565(40,40,40);
	this->backgroundColour_light = color565(60,60,60);
}

void GameColour::generateNewColour() {
	randomSeed(random()); // set seed for random() function, maybe shouldn't use random() for this but time or some more 'random' value 
	
	
	
	//this->setGameColour(random(100,255),random(100,255),random(100,255));
}

void GameColour::setGameColour(uint8_t red, uint8_t green, uint8_t blue) {
	this->gameColour = GameColour::color565(red, green, blue);
	this->gameColour_025 = GameColour::color565(0.25*red, 0.25*green, 0.25*blue);
	this->gameColour_050 = GameColour::color565(0.5*red, 0.5*green, 0.5*blue);
	this->gameColour_negative = GameColour::color565(255-red, 255-green, 255-blue);
	this->gameColour_negative_050 = GameColour::color565(0.5*(255-red), 0.5*(255-green), 0.5*(255-blue));
}

colour GameColour::color565(uint8_t red, uint8_t green, uint8_t blue) {
	return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

colour GameColour::getGameColour() {
	return this->gameColour;
}

colour GameColour::getGameColour_025() {
	return this->gameColour_025;
}

colour GameColour::getGameColour_050() {
	return this->gameColour_050;
}

colour GameColour::getGameColour_negative() {
	return this->gameColour_negative;
}

colour GameColour::getGameColour_negative_050() {
	return this->gameColour_negative_050;
}

colour GameColour::getBackgroundColour() {
	return this->backgroundColour;
}

colour GameColour::getBackgroundColour_light() {
	return this->backgroundColour_light;
}