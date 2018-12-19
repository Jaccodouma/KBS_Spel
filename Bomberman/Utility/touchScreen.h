// TouchScreen.h

#ifndef touchScreen_H
#define touchScreen_H

#include "../Utility/GameColour.h"
#include <ArduinoNunchuk.h>

// These defines throw up an error when used to create struct arrays
#define MAX_BUTTONS 10;
#define MAX_SLIDERS 5;

#define BUTTON_BORDER_WIDTH 3
#define BUTTON_ROUNDING 3

struct Button{
	uint16_t x, y, w, h;
	boolean *value;
	char text[21];
	uint8_t textSize;
	int offset_x, offset_y; 
};

struct Slider{
	uint16_t x, y, w, h;
	uint8_t *sliderValue;
};

/*
class Button
{};

class Button_press: public Button
{
public:

private:
	uint16_t x; 
	uint16_t y;
	uint16_t w; // width
	uint16_t h; // height
	
	char *text[21]; 
	uint8_t textSize;
	int text_offset_x;
	int text_offset_y;
	
	boolean *buttonValue; 
	
	Adafruit_ILI9341 *tft; 
	Adafruit_STMPE610 *touch;
	GameColour *gameColour; 
	
};*/

class TouchScreen
{
public:
	TouchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, ArduinoNunchuk *nunchuk);
	~TouchScreen();
	void handleInput(void);
	void newTextBotton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, boolean *buttonValue);
	void newCheckBox(uint16_t x, uint16_t y, uint8_t *checkBoxValue);
	void newSlider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *sliderValue);

private:
	// private functions
	void drawSelectedButton(uint8_t button);

	// other objects
	Adafruit_STMPE610 *touch;
	Adafruit_ILI9341 *tft;
	GameColour *gameColour;
	ArduinoNunchuk *nunchuk;
	
	// Arrays, the MAX_BUTTONS and MAX_SLIDERS defines seem to throw up errors
	Button *buttons[10];
	uint8_t buttonCount;
	Slider *sliders[5];
	uint8_t sliderCount;
	
	// Other values
	uint16_t x, y;
	uint8_t z;
	boolean isTouched;
	boolean wasTouched; 
	boolean usingNunchukSelection; // checks whether nunchuk selection is being used
	uint8_t selectedButton; 
	boolean selectionChanged;
	boolean usedNunchuk;
};

#endif