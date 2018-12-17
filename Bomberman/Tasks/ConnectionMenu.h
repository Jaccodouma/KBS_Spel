// ConnectionMenu.h

#ifndef _CONNECTIONMENU_h
#define _CONNECTIONMENU_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

#include "../Utility/GameColour.h"
#include "TaskManager.h"

class ConnectionMenu: public Task 
{
public: 
	ConnectionMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen, ArduinoNunchuk *nunchuk, GameColour *gameColour);
	int run(); 
private:
	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *TouchScreen;
	ArduinoNunchuk *nunchuk;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	GameColour *gameColour;
};

#endif

