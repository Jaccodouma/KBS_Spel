#include "GameColour.h"
#include <Arduino.h>

GameColour::GameColour() {
	generateNewColour();
	this->backgroundColour = color565(40,40,40);
	this->backgroundColour_light = color565(60,60,60);
}

void GameColour::generateNewColour() {
	//randomSeed(random()); // set seed for random() function, maybe shouldn't use random() for this but time or some more 'random' value 
	
	rgb newColourRGB =getRGBfromHue(random(0,360));
	
	uint8_t r,g,b;
	
	r = newColourRGB.r;
	g = newColourRGB.g;
	b = newColourRGB.b;
	
	this->setGameColour(r,g,b);
}

rgb GameColour::getRGBfromHue(uint16_t hue) {
	rgb out; 
	uint8_t segment;
	uint8_t slope;
	
	// filter the hue value, just in case (also takes care of 360 == 0)
	hue%=360;
	
	// get the slope value and map it to 0-255 range 
	slope = hue % 60 * (255/60);
	
	// Get segment, divide the hue into 6 segments (0 to 5)
	segment = hue / 60; // 360/6 = 60
	
	switch (segment) {
	case 0: 
		out.r = 255;
		out.g = slope;
		out.b = 0;
		break;
	case 1:
		out.r = 1-slope;
		out.g = 255;
		out.b = 0;
		break;
	case 2:
		out.r = 0;
		out.g = 255;
		out.b = slope;
		break;
	case 3:
		out.r = 0;
		out.g = 1-slope;
		out.b = 255;
		break;
	case 4:
		out.r = slope;
		out.g = 0;
		out.b = 255;
		break;
	case 5:
	default:
		out.r = 255;
		out.g = 0;
		out.b = 1-slope;
		break;
	}
	
	// Print output for debugging
	Serial.print("Hue: "); Serial.println(hue);
	//Serial.println(segment);
	Serial.print("R: "); Serial.println(out.r);
	Serial.print("G: "); Serial.println(out.g);
	Serial.print("B: "); Serial.println(out.b);
	
	return out;
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
