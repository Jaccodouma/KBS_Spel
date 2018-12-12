/*
    Name:       Bomberman.ino
    Created:	12/12/2018 12:02:28
    Author:     Jacco
*/

// Libraries for screen
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"	// TFT screen
#include "Adafruit_STMPE610.h"	// Touch screen

// default defines for ADAfruit shield
#define TFT_DC 9
#define TFT_CS 10

// General defines for screen
#define SCREEN_W 240 // Screen is 240 pixels wide
#define SCREEN_H 320 // Screen is 320 pixels high
#define SCREEN_BORDER_H (SCREEN_H-SCREEN_W)/2 // Border on top and bottom of screen, difference between width and height

#include "SPI.h"
#include <Wire.h>

// Self-made Libraries
#include "TaskManager.h"
#include "IR.h"

// Task classes
#include "Tasks/IntroScreen.h"

// Screen objects
Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC); 
Adafruit_STMPE610 TouchScreen = Adafruit_STMPE610();

class TestTask: public Task
{
	public:
	/* Constructor */
	TestTask(int number) {
		this->taskNumber = number;
	};
	int run() {
		// Print task and received number
		Serial.print("TASK: ");
		Serial.println(taskNumber);
		
		// Task is done, return TASK_BUSY if this function needs to run again
		return TASK_DONE;
	}
	
	private:
	int taskNumber;
};

int main(void) {
	init();
	
	// Set pin 10 to output for touchScreen
	DDRB |= (1<<DDB2);
	PORTB |= (1<<PORTB2);
	
	// Create TaskManager object
	TaskManager *taskManager = new TaskManager; 
	
	// Create Task objects
	Task *introScreen = new IntroScreen(&Screen, &TouchScreen);
	Task *dummyTask1 = new TestTask(1);
	Task *dummyTask2 = new TestTask(2);
	Task *dummyTask3 = new TestTask(3);
	Task *dummyTask4 = new TestTask(4);
	Task *dummyTask5 = new TestTask(5);
	Task *dummyTask6 = new TestTask(6);
	
	// Add tasks to taskManager
	taskManager->addTask(introScreen);
	taskManager->addTask(dummyTask1);
	taskManager->addTask(dummyTask2);
	
	// Start screen and make it black
	Screen.begin();
	Screen.fillScreen(Screen.color565(0,0,0));
	
	Serial.begin(9600);
	Serial.println("Starting loop");
	
	while(1) {
		Serial.println("Doing task");
		taskManager->doTask(); // do current task
	}
}
