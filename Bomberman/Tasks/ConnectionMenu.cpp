#include "ConnectionMenu.h"

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

// Constructor
ConnectionMenu::ConnectionMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *touch, ArduinoNunchuk *nunchuk, GameColour *gameColour) 
	: touchScreen(Screen, touch, gameColour, nunchuk) // initialize new touchScreen object
{
	this->Screen = Screen;
	this->touch = touch;
	this->nunchuk = nunchuk;
	this->gameColour = gameColour;
	this->initialised = false;
	this->createGame = false;
	this->joinGame = false;
}

ConnectionMenu::run() {
	if (!this->initialised) {
		// Draw the screen
		Screen->fillScreen(this->gameColour->getGameColour_050()); // Reset the screen to background colour
		
		// Draw background for the top bit
		Screen->fillRect(0, 0, Screen->width(), 50, gameColour->getBackgroundColour());
		Screen->drawLine(0, 46, Screen->width(), 46, gameColour->getGameColour());
		
		// Draw "Bombermenu" text
		Screen->setTextColor(gameColour->getGameColour());
		Screen->setTextSize(3);
		Screen->setCursor(35,20);
		Screen->print("BOMBERMENU");
		
		// Create buttons		  x   y   w    h   text      size	offset_x	offset_y	value	
		touchScreen.newTextBotton(20, 60, 200, 40, "CREATE", 3,		20,			0,			&createGame);
		touchScreen.newTextBotton(20, 105, 200, 40, "JOIN",	 3,		35,			0,			&joinGame);

		touchScreen.newSlider(20, 195, 200, 40, "SLIDERS", 3,	10,	0, &sliderValue);
		touchScreen.newToggleButton(20, 240, 200, 40, "TOGGLE", 3,	20,	0, &toggleValue);
		
		touchScreen.draw();
		
		this->initialised = true;
	}
	
	touchScreen.handleInput();
	
	// check buttons
	if (createGame) {
		Serial.println("create game");
		Screen->fillCircle(0,0,10,Screen->color565(255,0,0));
		createGame = false;
	}
	if (joinGame) {
		Serial.println("join game");
		Screen->fillCircle(240,0,10,Screen->color565(0,0,255));
		joinGame = false;
	}
	
	
	
	if (toggleValue) {
		// Toggled on
	} else {
		// Toggled off
	}
	
	return TASK_BUSY;
}