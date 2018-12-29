#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include "touchScreen.h"
#include "../Utility/GameColour.h"

TouchScreen::TouchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, ArduinoNunchuk *nunchuk) {  //constructor
	this->touch = touch;
	this->tft = tft;
	this->gameColour = gameColour;
	this->nunchuk = nunchuk;
	
	// set button & slider counters
	this->buttonCount = 0;
	
	this->isTouched = false;
	this->wasTouched = false; 
	this->usingNunchukSelection = false;
	this->selectedButton = 0; 
	this->usedNunchuk = false;
}

TouchScreen::~TouchScreen() {
	// DESTRUCTOR
	cleanUp();
}

void TouchScreen::handleInput(void){
	this->selectionChanged = false; // if true we need to redraw the buttons
	
	// TOUCH SCREEN
	// update x,y,z 
	if (touch->touched()) { // touchscreen got touched
		wasTouched = true; 
		
		// update x,y,z values
		while(!touch->bufferEmpty()) {
			touch->readData(&x,&y,&z);
		}
		
		// map x and y to usable values
		x = map(x, 230, 3750, tft->width(), 0);
		y = map(y, 150, 4000, tft->height(), 0);
		
// 		Serial.println("Touched:");
// 		Serial.print(x); Serial.print(" ");
// 		Serial.println(y); 
		
		// take x,y values and 'select' the correct button
		for (uint8_t i = 0; i<buttonCount; i++) {
			if ( // input values are between current button's x and y
			(x > buttons[i]->x) &&
			(x < buttons[i]->x + (buttons[i]->w)) &&
			(y > buttons[i]->y) &&
			(y < buttons[i]->y + (buttons[i]->h))
			) {
				selectedButton = i;
				selectionChanged = true;
				hasSelectedButton = true;
				
				// send touch input to button (for sliders etc.)
				buttons[selectedButton]->input_touch(x);
				
				break; // break out of the for loop
			}
			hasSelectedButton = false; // only reached if no button selected
		}
	} else { // !touch->touched()
		if (wasTouched) {
			wasTouched = false; // reset wasTouched so this only happens once
			if(hasSelectedButton) {
				buttons[selectedButton]->input();
				selectionChanged = true;
			}
		}
	}
	
	// NUNCHUK
	nunchuk->update();
	if (this->usingNunchukSelection) {
		// Check for input and handle it
		// TODO: Create some better way of taking input from nunchuk
		
		// If there's up/down or C/Z input
		if (
			nunchuk->analogY < 64 || nunchuk->analogY > 191 ||
			nunchuk->zButton || nunchuk->cButton
		) {
			if (!usedNunchuk) {
				usedNunchuk = true;
				// up
				if (nunchuk->analogY > 191) {
					if (selectedButton == 0) {
						selectedButton = buttonCount-1;
					} else {
						selectedButton--;
					}
					selectionChanged = true;
				}
						
				// down
				if (nunchuk->analogY < 64) {
					selectedButton++;
					selectedButton%=buttonCount;
					selectionChanged = true;
				}
				
				// Z/C button input, select button 			
				if (nunchuk->cButton || nunchuk->zButton) {
					// set selected button value to true
					this->selectionChanged = true; 
					buttons[selectedButton]->input();
				}
			}
		} else {
			// Set usedNunchuk back to false so next input gets recognized
			usedNunchuk = false;
			
			// do send nunchuk X data
			if (buttons[selectedButton]->input_nunchuk(nunchuk->analogX)) {
				selectionChanged = true;
			};
		}
	} else {
		// Check if nunchuk got any input and if there was, put usedNunchuk to true;
		if (
			nunchuk->analogX < 64 || nunchuk->analogX > 191 || // X
			nunchuk->analogY < 64 || nunchuk->analogY > 191 || // Y
			nunchuk->zButton || nunchuk->cButton // Buttons
		) {
			this->usingNunchukSelection = true;
			this->selectionChanged = true;
		}
	}
	
	if (this->selectionChanged) {
		this->drawSelectedButton(selectedButton);
		selectionChanged = false;
	}

}

void TouchScreen::draw() {
	// draw the buttons
	for (uint8_t i=0; i<buttonCount; i++) {
		buttons[i]->draw(false, true);
	}
}

void TouchScreen::cleanUp() {
	// Delete button objectss
	for (uint8_t i=0; i<buttonCount; i++) {
		delete buttons[i];
	}
	
	// Reset values
	this->buttonCount = 0;
	
	this->isTouched = false;
	this->wasTouched = false;
	this->usingNunchukSelection = false;
	this->selectedButton = 0;
	this->usedNunchuk = false;
}

void TouchScreen::newTextBotton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue) 
{
	//create button 
	Button_Push *newButton = new Button_Push(x, y, w, h, text, textSize, text_offset_x, text_offset_y, tft, touch, gameColour, buttonValue);

	//put button in array
	buttons[buttonCount] = newButton;
	buttonCount++;
}

void TouchScreen::newToggleButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue) 
{
	// create Button
	Button_Toggle *newButton = new Button_Toggle(x, y, w, h, text, textSize, text_offset_x, text_offset_y, tft, touch, gameColour, buttonValue);
		
	// put button in array
	buttons[buttonCount] = newButton;
	buttonCount++;
}

void TouchScreen::newSlider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue)
{
	// create Slider
	Button_Slider *newButton = new Button_Slider(x, y, w, h, text, textSize, text_offset_x, text_offset_y, tft, touch, gameColour, buttonValue);
	
	// put slider in array
	buttons[buttonCount] = newButton;
	buttonCount++;
}

void TouchScreen::drawSelectedButton(uint8_t buttonNumber) {
	// redraw buttons with the right one selected
	// TODO: make it so that it only redraws the two buttons that need redrawing
	for (uint8_t i=0; i<buttonCount; i++) {
		if (i==buttonNumber) {
			buttons[i]->draw(true, false);
		} else {
			buttons[i]->draw(false, false);
		}
	}
}

Button::Button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, 	Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue) {
	this->x = x; 
	this->y = y; 
	this->w = w; 
	this->h = h; 
	
	this->text[0] = '\0'; // make sure text is empty
	strcat(this->text, text);
	this->textSize = textSize; 
	this->text_offset_x = text_offset_x; 
	this->text_offset_y = text_offset_y;
	
	this->tft = tft; 
	this->touch = touch;
	this->gameColour = gameColour;
	this->buttonValue = buttonValue;
	
	this->buttonType = BTN_TYPE_PUSH;
}

Button_Push::Button_Push(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue) 
:Button(x,y,w,h,text,textSize,text_offset_x,text_offset_y,tft,touch,gameColour, buttonValue)
{}

void Button_Push::draw() 
{
	this->draw(false); // no arguments, button is not selected
}
void Button_Push::draw(boolean enabled) 
{
	this->draw(enabled, true);
}
void Button_Push::draw(boolean enabled, boolean drawBackGround) {
	colour accentColour;
	if (enabled) {
		accentColour = super::gameColour->getGameColour_negative();
		} else {
		accentColour = super::gameColour->getGameColour();
	}
		
	// background
	if (drawBackGround) {
		tft->fillRoundRect(x, y, w, h,/*radius: */ BUTTON_ROUNDING, gameColour->getBackgroundColour());
	}
		
	// outline
	tft->drawRoundRect(
		x+BUTTON_BORDER_WIDTH,
		y+BUTTON_BORDER_WIDTH,
		w-2*BUTTON_BORDER_WIDTH,
		h-2*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		accentColour
	);
		
	// text
	tft->setTextSize(textSize);
	tft->setTextColor(accentColour);
	tft->setCursor(x+(h/2)+5+text_offset_x, y+10+text_offset_y);
	tft->println(text);
}

void Button_Push::input() {
	*this->buttonValue = true;
}

Button_Slider::Button_Slider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue)
:Button(x,y,w,h,text,textSize,text_offset_x,text_offset_y,tft,touch,gameColour, buttonValue)
{
	this->oldValue = *buttonValue; // set oldValue to current buttonValue;
	this->sliderSpeed = 3;
	
	this->buttonType = BTN_TYPE_SLIDER;
}
	
void Button_Slider::draw() 
{
	this->draw(false); // no arguments, button is not selected
}
void Button_Slider::draw(boolean enabled) 
{
	this->draw(enabled, true); // button is enabled, draw background
}
void Button_Slider::draw(boolean enabled, boolean drawBackGround)
{
	// get accent colours
	colour accentColour;
	colour accentColour_dark;
	if (enabled) {
		accentColour = super::gameColour->getGameColour_negative();
		accentColour_dark = super::gameColour->getGameColour_negative_050();
		} else {
		accentColour = super::gameColour->getGameColour();
		accentColour_dark = super::gameColour->getGameColour_050();
	}
	
	// background
	if (drawBackGround) {
		tft->fillRoundRect(x, y, w, h,/*radius: */ BUTTON_ROUNDING, gameColour->getBackgroundColour());
	}
	
	// Overwrite old slider if buttonValue changed
	if (this->oldValue != *this->buttonValue) {
		uint16_t oldSlider_x = x+BUTTON_BORDER_WIDTH + ((w-2*BUTTON_BORDER_WIDTH-SLIDER_SIZE) * oldValue / 255);
		
		tft->fillRect(
			/* x */ oldSlider_x,
			/* y */ y+BUTTON_BORDER_WIDTH,
			/* w */ SLIDER_SIZE, 
			/* h */ h-2*BUTTON_BORDER_WIDTH,
			/*clr*/ gameColour->getBackgroundColour()
		);
	
		this->oldValue = *this->buttonValue; // set oldValue
	}
	
	// Slider
	uint16_t slider_x = x+BUTTON_BORDER_WIDTH + ((w-2*BUTTON_BORDER_WIDTH-SLIDER_SIZE) * *buttonValue / 255); // the * with "*buttonValue" is dereference
	tft->fillRoundRect(
		slider_x,
		y+BUTTON_BORDER_WIDTH,
		SLIDER_SIZE,
		h-2*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		accentColour_dark
	);
	
	// Button outline
	tft->drawRoundRect(
		x+BUTTON_BORDER_WIDTH,
		y+BUTTON_BORDER_WIDTH,
		w-2*BUTTON_BORDER_WIDTH,
		h-2*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		gameColour->getGameColour()
	);
	
	// Slider outline
	tft->drawRoundRect(
		slider_x,
		y+BUTTON_BORDER_WIDTH,
		SLIDER_SIZE,
		h-2*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING+1,
		accentColour
	);
	tft->drawRoundRect(
		slider_x+1,
		y+BUTTON_BORDER_WIDTH+1,
		SLIDER_SIZE-2,
		h-2*BUTTON_BORDER_WIDTH-2,
		BUTTON_ROUNDING,
		accentColour
	);
	
	// text
	tft->setTextSize(textSize);
	tft->setTextColor(gameColour->getGameColour());
	tft->setCursor(x+(h/2)+5+text_offset_x, y+10+text_offset_y);
	tft->println(text);
}

boolean Button_Slider::input_nunchuk(uint8_t analogX) {
	/* this takes the X input from the nunchuk
	 * Which technically ranges from 0 to 255, but in reality
	 * it's more like 38 - 238
	 * 
	 * So we take input as follows: 
	 *   35 - 115 (80 wide) --> Lower bit of the input
	 *  115 - 155 (20 wide) --> Deadzone
	 *  155 - 235 (80 wide) --> Upper bit of the input
	 */
	boolean inputChanged = false;
	
	/* *this->buttonValue++ or -- doesn't work for some reason */
	// down
	if (analogX<115 && *this->buttonValue != 0) {
		if (*this->buttonValue > sliderSpeed) {
			*this->buttonValue= *this->buttonValue-sliderSpeed;
		} else {
			*this->buttonValue = 0;
		}
		inputChanged = true;
	}
	// up
	if (analogX>155 && *this->buttonValue != 255) {
		if (*this->buttonValue < 255-sliderSpeed) {
			*this->buttonValue= *this->buttonValue+sliderSpeed;
		} else {
			*this->buttonValue = 255;
		}
		
		
		inputChanged = true;
	}
	
	return inputChanged;
}
boolean Button_Slider::input_touch(uint16_t touchX) {
	Serial.println(touchX);
	
	// map the input value to a button value 
	*this->buttonValue = map(
		touchX,		/* value */ 
		x,			/* fromLow */ 
		x+w,		/* fromHigh */ 
		0,			/* toLow */ 
		255			/* toHigh */ 
	);
	
	return true; 
}

Button_Toggle::Button_Toggle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue)
:Button(x,y,w,h,text,textSize,text_offset_x,text_offset_y,tft,touch,gameColour, buttonValue)
{}

void Button_Toggle::draw()
{
	this->draw(false); // no arguments, button is not selected
}
void Button_Toggle::draw(boolean enabled)
{
	this->draw(enabled, true); // button is enabled, draw background
}
void Button_Toggle::draw(boolean enabled, boolean drawBackGround)
{
	colour accentColour;
	colour selectorColour; 
	if (enabled) {
		accentColour = super::gameColour->getGameColour_negative();
		
		if (*this->buttonValue) {
			selectorColour = super::gameColour->getGameColour_negative();
		} else {
			selectorColour = super::gameColour->getGameColour_negative_050();
		}
		
	} else {
		accentColour = super::gameColour->getGameColour();
		
		if (*this->buttonValue) {
			selectorColour = super::gameColour->getGameColour();
		} else {
			selectorColour = super::gameColour->getGameColour_050();
		}
	}
		
	// background
	if (drawBackGround) {
		tft->fillRoundRect(x, y, w, h,/*radius: */ BUTTON_ROUNDING, gameColour->getBackgroundColour());
	}
		
	// outline
	tft->drawRoundRect(
		x+BUTTON_BORDER_WIDTH,
		y+BUTTON_BORDER_WIDTH,
		w-2*BUTTON_BORDER_WIDTH,
		h-2*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		gameColour->getGameColour() //accentColour
	);
	
	// Indicator
	tft->fillRoundRect( // background
		x+2*BUTTON_BORDER_WIDTH,
		y+2*BUTTON_BORDER_WIDTH,
		h-4*BUTTON_BORDER_WIDTH,
		h-4*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		gameColour->getBackgroundColour_light()
	);
	tft->drawRoundRect( // outline
		x+2*BUTTON_BORDER_WIDTH,
		y+2*BUTTON_BORDER_WIDTH,
		h-4*BUTTON_BORDER_WIDTH,
		h-4*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		accentColour
	);
	
	tft->fillRoundRect( // selector
		x+3*BUTTON_BORDER_WIDTH,
		y+3*BUTTON_BORDER_WIDTH,
		h-6*BUTTON_BORDER_WIDTH,
		h-6*BUTTON_BORDER_WIDTH,
		BUTTON_ROUNDING,
		selectorColour
	);
		
	// text
	tft->setTextSize(textSize);
	tft->setTextColor(gameColour->getGameColour());
	tft->setCursor(x+(h/2)+5+text_offset_x, y+10+text_offset_y);
	tft->println(text);
}

void Button_Toggle::input() {
	*this->buttonValue = !*this->buttonValue; 
}