#include "IntroScreen.h"

#include "Adafruit_ILI9341.h"	// TFT screen
#include <ArduinoNunchuk.h>		// Nunchuk

// Constructor
IntroScreen::IntroScreen(Adafruit_ILI9341 *Screen, ArduinoNunchuk *nunchuk, GameColour *gameColour) {
	this->Screen = Screen;
	this->nunchuk = nunchuk;
	this->gameColour = gameColour;
	this->initialised = false;
}

IntroScreen::run() {
	if (!this->initialised) {
		Serial.println("Drawing screen");
		
		// Draw the screen
		Screen->fillScreen(this->gameColour->getGameColour());
		
		// Draw "Bomberman" text
		Screen->setTextColor(gameColour->getGameColour_050());
		Screen->setTextSize(4);
		Screen->setCursor(13,39);
		Screen->print("BOMBERMAN");
		Screen->setTextColor(gameColour->getGameColour_negative());
		Screen->setTextSize(4);
		Screen->setCursor(14,40);
		Screen->print("BOMBERMAN");
		
		// Draw other text
		Screen->setTextSize(2);
	
		
		Screen->setTextColor(gameColour->getGameColour_050());
		Screen->setCursor(24,259);
		Screen->print("C: change colour");
		Screen->setTextColor(gameColour->getGameColour_negative());
		Screen->setCursor(25,260);
		Screen->print("C: change colour");
		
		Screen->setTextColor(gameColour->getGameColour_050());
		Screen->setCursor(56,279);
		Screen->print("Z: continue");
		Screen->setTextColor(gameColour->getGameColour_negative());
		Screen->setCursor(57,280);
		Screen->print("Z: continue");
		
		
		this->initialised = true;
	}
	// Check if Zbutton is pressed on nunchuck. If it is, return TASK_DONE
	nunchuk->update();
	if (nunchuk->zButton) {
		this->initialised = false;
		return TASK_DONE;
	}
		
	// Check if Cbutton is pressed on nunchuck. If it is, regenerate colour and redraw the screen
	if (nunchuk->cButton) {
		this->gameColour->generateNewColour();
		this->initialised = false;
	}
	
	return TASK_BUSY;
}
