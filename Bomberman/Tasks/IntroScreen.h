// IntroScreen.h

#ifndef _INTROSCREEN_h
#define _INTROSCREEN_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch
#include <ArduinoNunchuk.h>		// Nunchuk

#include "TaskManager.h"

// Typedefs
typedef uint16_t colour;

class IntroScreen: public Task
{
public:
	IntroScreen(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen, ArduinoNunchuk *nunchuk, colour *gameColour);
	int run();
private:
	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *TouchScreen;
	ArduinoNunchuk *nunchuk;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	colour *gameColour;
	uint16_t x, y;
	uint8_t z;
};

#endif

