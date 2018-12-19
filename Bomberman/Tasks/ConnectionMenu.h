// ConnectionMenu.h

#ifndef _CONNECTIONMENU_h
#define _CONNECTIONMENU_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

#include "../Utility/GameColour.h"
#include "../Utility/touchScreen.h"
#include "TaskManager.h"

class ConnectionMenu: public Task 
{
public: 
	ConnectionMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *touch, ArduinoNunchuk *nunchuk, GameColour *gameColour);
	int run(); 
private:
	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *touch;
	ArduinoNunchuk *nunchuk;
	
	TouchScreen touchScreen;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	GameColour *gameColour;
	
	uint8_t createGame;
	uint8_t joinGame;
};

#endif

