#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "../Utility/touchScreen.h"

#include "settingMenu.h"

SettingMenu::SettingMenu(Adafruit_ILI9341 *Screen, Adafruit_STMPE610 *touch, ArduinoNunchuk *nunchuk, GameColour *gameColour) 
: touchScreen(Screen, touch, gameColour, nunchuk) // initialize new touchScreen object
{
	this->Screen = Screen;
	this->touch = touch;
	this->nunchuk = nunchuk;
	this->gameColour = gameColour;
	this->initialised = false;
	
	// Initialize PWM for screen brightness
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS02); //set clock prescaler 1024 and PWM, Phase and Frequency Correct
	DDRD = (1 << DDD6); //set OC0A as output PORTD6 pin6
	
	// Initialize Analog-Digital Conversion for taking input from the light sensor
	ADMUX = (1 << 0) | (1 << REFS0); //internal 5v reference and A1 as input
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //ADCclock=CPUclock/128
	ADCSRA |= (1 << ADEN);  //ADC enabled
	DDRC &= ~(1 << DDC1); // set A1 as input
	PORTC &= ~(1 << PORTC1); // A1 pulldown off
	
	// Set output compare to current brightness 
	OCR0A = brightness_slider; // (this sets the brightness)
}

int SettingMenu::run() {
	if (!this->initialised) {
		// Draw the screen
		Screen->fillScreen(this->gameColour->getGameColour_050()); // Reset the screen to background colour
				
		// Draw background for the top bit
		Screen->fillRect(0, 0, Screen->width(), 50, gameColour->getBackgroundColour());
		Screen->drawLine(0, 46, Screen->width(), 46, gameColour->getGameColour());
				
		// Draw "Bombermenu" text
		Screen->setTextColor(gameColour->getGameColour());
		Screen->setTextSize(3);
		Screen->setCursor(35,20);
		Screen->print("SETTINGS");
		
		// Create buttons 
		touchScreen.newToggleButton(20,60,200,40,"AUTO",3,15,0,&autoBrightness);
		//touchScreen.newSlider(20,105,200,40,"BRIGHTNESS",3,-15,0,&brightness_slider);
		
		touchScreen.newTextBotton(20,240,100,40,"OK",3,20,0,&button_ok);
		
		touchScreen.draw();
		
		this->initialised = true; 
	}
	
	touchScreen.handleInput();
	
	if (button_ok) { // if OK was pressed, task is done 
		this->initialised = false;
		this->touchScreen.cleanUp();
		return TASK_DONE;
	}

	return TASK_BUSY;
}

void SettingMenu::updateScreenBrightness()
{
	uint8_t desiredBrightness = brightness_slider; 
	if (autoBrightness) {
		// get desired brightness from the sensor
		desiredBrightness = getSensorDesiredBrightness();
		brightness_slider = desiredBrightness; 
	} 
	
	// set Brightness
	OCR0A = desiredBrightness;
}

uint8_t SettingMenu::getSensorDesiredBrightness()
{
	ADCSRA |= (1 << ADSC);        //single AD-conversion
	while (ADCSRA & (1 << ADSC)); //wait for result..
	
	return (uint8_t) map(ADC, 0, 1023, 0, 255);
}