#ifndef settingMenu_H
#define settingMenu_H

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

#include "touchScreen.h"
#include "GameColour.h"
#include "TaskManager.h"

class SettingMenu: public Task
{
public:
	SettingMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *touch, ArduinoNunchuk *nunchuk, GameColour *gameColour);
	int run(void);
	void updateScreenBrightness(); // Called in main loop, updates screen brightness automatically
	
private:
	uint8_t getSensorDesiredBrightness(); // gets desired brightness from sensor

	Adafruit_ILI9341 *Screen;
	Adafruit_STMPE610 *touch;
	ArduinoNunchuk *nunchuk;
	
	TouchScreen touchScreen;
	
	boolean initialised; // if initialised the screen is showing the right stuff
	GameColour *gameColour;

	uint8_t autoBrightness = true;
	uint8_t brightness_slider = 150;
	uint8_t brightness_old = 150;
	
	uint8_t button_ok = false;

};

#endif
