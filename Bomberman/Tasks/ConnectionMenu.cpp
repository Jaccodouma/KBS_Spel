#include "ConnectionMenu.h"

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

// Constructor
ConnectionMenu::ConnectionMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen, ArduinoNunchuk *nunchuk, colour *gameColour) {
	this->Screen = Screen;
	this->TouchScreen = TouchScreen;
	this->nunchuk = nunchuk;
	this->gameColour = gameColour;
	this->initialised = false;
}

ConnectionMenu:: run() {
	if (!this->initialised) {
		// Draw the screen
		Screen->fillScreen(this->gameColour); // Reset the screen to background colour
		
		// Draw "Bombermenu" text
		Screen->setTextColor(Screen->color565(0,0,0));
		Screen->setTextSize(3);
		Screen->setCursor(35,20);
		Screen->print("BOMBERMENU");
		
		this->initialised = true;
	}
	
	return TASK_BUSY;
}