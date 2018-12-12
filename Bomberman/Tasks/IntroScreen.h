// IntroScreen.h

#ifndef _INTROSCREEN_h
#define _INTROSCREEN_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

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
};

#endif

