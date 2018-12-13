#include "IntroScreen.h"

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen
#include <ArduinoNunchuk.h>		// Nunchuk

// Constructor
IntroScreen::IntroScreen(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen, ArduinoNunchuk *nunchuk, colour *gameColour) {
	this->Screen = Screen;
	this->TouchScreen = TouchScreen;
	this->nunchuk = nunchuk;
	this->gameColour = gameColour;
	this->initialised = false;
}

IntroScreen::run() {
	if (!this->initialised) {
		// Draw the screen
		Screen->fillScreen(this->gameColour);
		
		// Draw "Bomberman" text
		Screen->setTextColor(Screen->color565(0,0,0));
		Screen->setTextSize(4);
		Screen->setCursor(14,40);
		Screen->print("BOMBERMAN");
		
		// Draw other text
		Screen->setTextSize(2);
		Screen->setCursor(25,260);
		Screen->print("C: change colour");
		Screen->setCursor(57,280);
		Screen->print("Z: continue");
		
		Serial.println("IntroScreen.run() - not initialised");
		
		this->initialised = true;
		} else {
		// Check if a button is pressed. If it is, return TASK_DONE
		// TODO: fix this, touch screen doesn't work for some odd reason
		if (TouchScreen->touched()) {
			Serial.println("Touchscreen touchie");
			while (!TouchScreen->bufferEmpty()) {
				TouchScreen->readData(&x, &y, &z);
			}
			TouchScreen->writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
			
			this->initialised = false;
			return TASK_DONE;
		}
		
		// Check if Zbutton is pressed on nunchuck. If it is, return TASK_DONE
		nunchuk->update();
		if (nunchuk->zButton) {
			this->initialised = false;
			return TASK_DONE;
		}
		
		// Check if Cbutton is pressed on nunchuck. If it is, regenerate colour and redraw the screen
		if (nunchuk->cButton) {
			this->gameColour = Screen->color565(random(100,255),random(100,255),random(100,255));
			this->initialised = false;
		}
	}
	return TASK_BUSY;
}
