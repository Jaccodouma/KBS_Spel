// IntroScreen.h

#ifndef _INTROSCREEN_h
#define _INTROSCREEN_h

#include "TaskManager.h"

class IntroScreen: public Task
{
public:
	IntroScreen(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen);
	int run();
private: 
	int initialised; // if initialised the screen is showing the right stuff 
	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *TouchScreen;
	uint16_t x, y;
	uint8_t z;
};

#endif

