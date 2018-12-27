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

// Button type defines
#define BTN_TYPE_PUSH 0 
#define BTN_TYPE_SLIDER 1

class Button
{
public:
	Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, 	Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	virtual void draw();
	virtual void draw(boolean enabled);
	virtual void draw(boolean enabled, boolean drawBackground);
	
	virtual void input() = 0; // General input for push buttons
	virtual boolean input_nunchuk(uint8_t analogX) = 0; // Handles nunchuk input (left/right) for sliders, returns whether the value was changed
	virtual boolean input_touch(uint16_t touchX) = 0; // Handles touch input for sliders, returns whether the value was changed
	
	uint8_t buttonType; 
	
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
	void draw();
	void draw(boolean enabled);
	void draw(boolean enabled, boolean drawBackGround);
	
	void input(); // Input 
	boolean input_nunchuk(uint8_t analogX) {return false;}; // not a slider, no horizontal input
	boolean input_touch(uint16_t touchX) {return false;}; // not a slider, don't need touch input
private:
};

class Button_Slider: public Button 
{
public:
	Button_Slider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	void draw();
	void draw(boolean enabled);
	void draw(boolean enabled, boolean drawBackGround);
	
	void input() {}; // Not a push button, no boolean input 
	boolean input_nunchuk(uint8_t analogX); // Handles nunchuk input 
	boolean input_touch(uint16_t touchX); // Handles touch input
	
	uint8_t buttonType = BTN_TYPE_SLIDER;
private:
	uint8_t oldValue; 
	uint8_t sliderSpeed; 
};

class Button_Toggle: public Button 
{
public: 
	Button_Toggle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue);
	void draw();
	void draw(boolean enabled);
	void draw(boolean enabled, boolean drawBackGround);
	
	void input(); // Input
	boolean input_nunchuk(uint8_t analogX) {return false;}; // not a slider, no horizontal input
	boolean input_touch(uint16_t touchX) {return false;}; // not a slider, don't need touch input
};

class TouchScreen
{
public:
	TouchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, ArduinoNunchuk *nunchuk);
	~TouchScreen();
	void handleInput();
	void draw();
	void newTextBotton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);
	void newToggleButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);
	void newSlider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue);

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
	
	// Other values
	uint16_t x, y;
	uint8_t z;
	boolean isTouched;
	boolean wasTouched; 
	boolean usingNunchukSelection; // checks whether nunchuk selection is being used
	uint8_t selectedButton; 
	boolean hasSelectedButton; 
	boolean selectionChanged;
	boolean usedNunchuk;
};

#endif