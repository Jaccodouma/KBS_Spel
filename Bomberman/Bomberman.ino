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

// Self-made Libraries
#include "TaskManager.h"
#include "IR.h"

// Task classes
#include "Tasks/IntroScreen.h"

// Typedefs
typedef uint16_t colour; 

// Screen objects
Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC); 
Adafruit_STMPE610 TouchScreen = Adafruit_STMPE610(8);

// Nunchuk object
ArduinoNunchuk nunchuk = ArduinoNunchuk();

int main(void) {
	init();
	nunchuk.init();
	
	// Generate game colour
	colour gameColour = Screen.color565(random(100,255),random(100,255),random(100,255));
	
	// Set pin 10 to output for touchScreen
	DDRB |= (1<<DDB2);
	PORTB |= (1<<PORTB2);
	
	// Create TaskManager object
	TaskManager *taskManager = new TaskManager; 
	
	// Create Task objects
	Task *introScreen = new IntroScreen(&Screen, &TouchScreen, &nunchuk, &gameColour);
	
	// Add tasks to taskManager
	taskManager->addTask(introScreen);
	
	
	// Start screen and make it the right colour
	Screen.begin();
	Screen.setRotation(2);
	Screen.fillScreen(gameColour);
	
	Serial.begin(9600);
	Serial.println("Starting loop");
	
	while(1) {
		taskManager->doTask(); // do current task
	}
}
