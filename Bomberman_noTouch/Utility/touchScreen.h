// TouchScreen.h

#ifndef touchScreen_H
#define touchScreen_H

#include "../Utility/GameColour.h"
#include <ArduinoNunchuk.h>

// These defines throw up an error when used to create struct arrays
#define MAX_BUTTONS 10;
#define MAX_SLIDERS 5;

// Size defines
#define BUTTON_BORDER_WIDTH 3
#define BUTTON_ROUNDING 3

#define SLIDER_SIZE 25
#define SLIDER_SPEED 3

// Button type defines
#define BTN_TYPE_PUSH 0 
#define BTN_TYPE_SLIDER 1

class Button
{
public:
	Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, 	Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	virtual void draw(boolean enabled, boolean drawBackground);
	
	void input() { *this->buttonValue = !*this->buttonValue; }; // General input for push buttons
	//virtual boolean input_nunchuk(uint8_t analogX) {return false;}; // Handles nunchuk input (left/right) for sliders, returns whether the value was changed
	//virtual boolean input_touch(uint16_t touchX) {return false;}; // Handles touch input for sliders, returns whether the value was changed
	
	//uint8_t buttonType; 
	
	uint16_t x;
	uint16_t y;
	uint16_t w; // width
	uint16_t h; // height
	
	uint8_t *buttonValue;
protected:
	typedef Button super; // Child classes can use super to call to parent class (:
	
	char text[21];
	uint8_t textSize;
	int text_offset_x;
	int text_offset_y;
	
	Adafruit_ILI9341 *tft;
	Adafruit_STMPE610 *touch;
	GameColour *gameColour;
};

class Button_Push: public Button
{
public:
	Button_Push(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	void draw(boolean enabled, boolean drawBackGround);
private:
};
/*
class Button_Slider: public Button 
{
public:
	Button_Slider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	void draw(boolean enabled, boolean drawBackGround);
	
	void input() {}; // Not a push button, no boolean input 
	boolean input_nunchuk(uint8_t analogX); // Handles nunchuk input 
	boolean input_touch(uint16_t touchX); // Handles touch input
private:
	uint8_t oldValue; 
};
*/
class Button_Toggle: public Button 
{
public: 
	Button_Toggle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	void draw(boolean enabled, boolean drawBackGround);
};

class TouchScreen
{
public:
	TouchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, ArduinoNunchuk *nunchuk);
	~TouchScreen();
	void handleInput(); // Handles input, should be called in a loop whenever buttons should be checked
	void draw(); // Draws the buttons, should be called after adding buttons
	void cleanUp(); // Cleans up memory used, call before going to the next task if you don't delete touchScreen object
					// Destructor calls this
	void newTextBotton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);
	void newToggleButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);
	//void newSlider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);

private:
	// private functions
	void drawSelectedButton(uint8_t button);

	// other objects
	Adafruit_STMPE610 *touch;
	Adafruit_ILI9341 *tft;
	GameColour *gameColour;
	ArduinoNunchuk *nunchuk;
	
	// Arrays, the MAX_BUTTONS define seems to throw up errors
	Button *buttons[10];
	uint8_t buttonCount;
	
	// Other values
	//uint16_t x, y;
	//uint8_t z;
	//boolean wasTouched; 
	boolean usingNunchukSelection; // checks whether nunchuk selection is being used
	uint8_t selectedButton; 
	boolean hasSelectedButton; 
	boolean selectionChanged;
	boolean usedNunchuk;
};

#endif