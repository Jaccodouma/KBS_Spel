#include "IntroScreen.h"

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

// Constructor 
IntroScreen::IntroScreen(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *TouchScreen) {
	this->Screen = Screen;
	this->TouchScreen = TouchScreen;
	this->initialised = 0;
}

IntroScreen::run() {
	if (!this->initialised) {
		// Draw the screen
		Screen->fillScreen(Screen->color565(random(100,255),0,random(100,255)));
		
		// Draw "Bomberman" text
		Screen->setTextColor(Screen->color565(0,0,0));
		Screen->setTextSize(4);
		Screen->setCursor(14,40);
		Screen->print("BOMBERMAN");
		
		// Draw other text
		Screen->setTextSize(2);
		Screen->setCursor(30,260);
		Screen->print("tap anywhere to");
		Screen->setCursor(70,280);
		Screen->print("continue");
		
		Serial.println("IntroScreen.run() - not initialised");
		
		this->initialised = 1;
	} else {
		Serial.println("IntroScreen.run() - initialised");
		// Check if the screen is pressed. If it is, return TASK_DONE
		if (!TouchScreen->bufferEmpty()) {
			Serial.println("Touchscreen is touched");
			this->initialised = 0;
			return TASK_DONE;
		} else {
			Serial.println("No Touchie");
		}
	}
	return TASK_BUSY;
}
