/*
#ifndef settingMenu_H
#define settingMenu_H

#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

#include "../Utility/touchScreen.h"
#include "../Utility/GameColour.h"
#include "TaskManager.h"

class SettingMenu: public Task
{
public:
	SettingMenu(TouchScreen *ts);
	int run(void);
	void screenBrightness(void);
	
private:
	void PWM_init(void);
	void adc_init(void);
	uint8_t screenbrightnessAuto(void);

	uint8_t photocellReading;
	uint8_t brightness = 150;
	uint8_t brightness_old = 150;

	TouchScreen *ts;
	Adafruit_STMPE610 *touch;
	Adafruit_ILI9341 *tft;
};

#endif
*/