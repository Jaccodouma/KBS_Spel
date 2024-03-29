// IntroScreen.h

#ifndef _INTROSCREEN_h
#define _INTROSCREEN_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include <ArduinoNunchuk.h>		// Nunchuk

#include "GameColour.h"
#include "TaskManager.h"

class IntroScreen: public Task
{
public:
	IntroScreen(Adafruit_ILI9341 *Screen, ArduinoNunchuk *nunchuk, GameColour *gameColour);
	int run();
private:
	Adafruit_ILI9341 *Screen;
	ArduinoNunchuk *nunchuk;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	GameColour *gameColour;
	uint16_t x, y;
	uint8_t z;
};

#endif
