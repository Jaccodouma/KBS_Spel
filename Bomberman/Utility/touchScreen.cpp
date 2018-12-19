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
	// TODO: free used memory (buttons)
}

void TouchScreen::handleInput(void){
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
		
		Serial.println("Touched:");
		Serial.print(x); Serial.print(" ");
		Serial.println(y); 
		
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
			}
		}
	} else { // !touch->touched()
		if (wasTouched) {
			wasTouched = false; // reset wasTouched so this only happens once
			*buttons[selectedButton]->buttonValue = true; 
		}
	}
	
	// NUNCHUK
	nunchuk->update();
	if (this->usingNunchukSelection) {
		// Check for input and handle it
		// TODO: Create some better way of taking input from nunchuk
		if ((nunchuk->analogY < 64 || nunchuk->analogY > 191 || nunchuk->zButton || nunchuk->cButton) ) {
			if (!usedNunchuk) {
				usedNunchuk = true;
				// up
				if (nunchuk->analogY > 191) {
					selectedButton--;
					selectedButton%=buttonCount;
					selectionChanged = true;
				}
						
				// down
				if (nunchuk->analogY < 64) {
					selectedButton++;
					selectedButton%=buttonCount;
					selectionChanged = true;
				}
				
				if (nunchuk->cButton || nunchuk->zButton) {
					// make selection
					*buttons[selectedButton]->buttonValue = true;
				}
			}
		} else {
			// Set usedNunchuk back to false so next input gets recognised
			usedNunchuk = false;
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

void TouchScreen::newTextBotton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char* text, uint8_t textSize, int text_offset_x, int text_offset_y, uint8_t *buttonValue) {
	//create button 
	Button_press *newButton = new Button_press(x, y, w, h, text, textSize, text_offset_x, text_offset_y, tft, touch, gameColour, buttonValue);

	//put struct in array
	buttons[buttonCount] = newButton;
	buttonCount++;
}

void TouchScreen::newCheckBox(uint16_t x, uint16_t y, uint8_t *checkBoxValue) {
	//draw button on screen

	//create button struct

	//put struct in array
}

void TouchScreen::newSlider(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *sliderValue) {
	//draw button on screen

	//create button struct

	//put struct in array
}

void TouchScreen::drawSelectedButton(uint8_t button) {
	// redraw buttons with the right one selected
	for (uint8_t i=0; i<buttonCount; i++) {
		if (i==button) {
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
}

Button_press::Button_press(uint16_t x, uint16_t y, uint16_t w, uint16_t h, char *text, uint8_t textSize, int text_offset_x, int text_offset_y, Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch, GameColour *gameColour, uint8_t *buttonValue) 
:Button(x,y,w,h,text,textSize,text_offset_x,text_offset_y,tft,touch,gameColour, buttonValue)
{}

void Button_press::draw() {
	this->draw(false);
}

void Button_press::draw(boolean enabled) {
this->draw(enabled, true);
}

void Button_press::draw(boolean enabled, boolean drawBackGround) {
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