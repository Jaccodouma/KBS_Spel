// ConnectionMenu.h

#ifndef _CONNECTIONMENU_h
#define _CONNECTIONMENU_h

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

#include "TaskManager.h"

// Typedefs
typedef uint16_t colour;

class ConnectionMenu: public Task 
{
public: 
	ConnectionMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen, ArduinoNunchuk *nunchuk, colour *gameColour);
	int run(); 
private:
	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *TouchScreen;
	ArduinoNunchuk *nunchuk;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	colour gameColour;
};

#endif

