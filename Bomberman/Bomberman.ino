
/*
    Name:       Bomberman.ino
    Created:	12/12/2018 12:02:28
    Author:     Jacco
*/

// Libraries for screen
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

// Nunchuk library
#include <ArduinoNunchuk.h>

// default defines for ADAfruit shield
#define TFT_DC 9
#define TFT_CS 10

// General defines for screen
#define SCREEN_W 240 // Screen is 240 pixels wide
#define SCREEN_H 320 // Screen is 320 pixels high

// Other libraries
#include "SPI.h"
#include <Wire.h>
#include <stdlib.h> // for the random() function

// Self-made Libraries and utilities
#include "TaskManager.h"
#include "IR.h"
#include "Utility/touchScreen.h"
#include "Utility/GameColour.h"

// Task classes
#include "Tasks/IntroScreen.h"
#include "Tasks/ConnectionMenu.h"
#include "Tasks/SettingMenu.h"

// Screen objects
Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC); 
Adafruit_STMPE610 Touch = Adafruit_STMPE610(8);

// Nunchuk object
ArduinoNunchuk nunchuk = ArduinoNunchuk();

int main(void) {
	
	// Nunchuk power, needed when it's on A2-5
	DDRC |= (1<<DDC2) | (1<<DDC3); // Set PC2 & PC3 on OUTPUT
	PORTC &= ~(1<<PORTC2); // set PC2 to LOW
	PORTC |= (1<<PORTC3); // Set PC3 to HIGH
	
	init();
	nunchuk.init();
	
	// Create myTS object
	touchScreen myTS(&Screen, &Touch);
	
	// Generate game colour
	GameColour gameColour;
	
	// Set pin 10 to output for touchScreen
	DDRB |= (1<<DDB2);
	PORTB |= (1<<PORTB2);
	
	// Create TaskManager object
	TaskManager *taskManager = new TaskManager; 
	
	// Create Task objects
	IntroScreen *introScreen = new IntroScreen(&Screen, &Touch, &nunchuk, &gameColour);
	ConnectionMenu *connectionMenu = new ConnectionMenu(&Screen, &Touch, &nunchuk, &gameColour);
	SettingMenu *settingMenu = new SettingMenu(&myTS);
	
	// Add tasks to taskManager
	taskManager->addTask(introScreen);
	taskManager->addTask(settingMenu);
	taskManager->addTask(connectionMenu);
	
	
	// Start screen and make it the right colour
	Screen.begin();
	Screen.setRotation(2);
	Screen.fillScreen(gameColour.getGameColour());
	
	Serial.begin(9600);
	Serial.println("Starting loop");
	
	long int highest = 0; 
	long int lowest = 99999999999999999999;
	
	while(1) {
		settingMenu->screenBrightness(); // update screen brightness
		taskManager->doTask(); // do current task
	}
}
