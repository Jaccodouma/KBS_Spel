/*
    Name:       Bomberman.ino
    Created:	12/12/2018 12:02:28
    Author:     Jacco & Christiaan
*/

// Libraries for screen
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"  // TFT screen
#include "Adafruit_STMPE610.h" // Touch screenxgyggyygyg

// Other libraries
#include "SPI.h"
#include <Wire.h>
#include <stdlib.h> // for the random() function

// Self-made Libraries and utilities
#include "TaskManager.h"
#include "IR.h"
#include "touchScreen.h"
#include "GameColour.h"
#include "Game.h"
#include "link.h"

// Task classes
#include "IntroScreen.h"
#include "SettingMenu.h"
#include "control.h"

// Nunchuk library
#include <ArduinoNunchuk.h>

// default defines for ADAfruit shield
#define TFT_DC 9
#define TFT_CS 10

// General defines for screen
#define SCREEN_W 240 // Screen is 240 pixels wide
#define SCREEN_H 320 // Screen is 320 pixels high

// Screen objects
Adafruit_ILI9341 Screen = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 Touch = Adafruit_STMPE610(8);

// Nunchuk object
ArduinoNunchuk nunchuk = ArduinoNunchuk();

#define MASTER 1

IR myIR(38, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect)
{
  myIR.timerOverflow();
}

ISR(PCINT2_vect)
{
  myIR.pinChange();
}

unsigned long update_timer;

int main(void)
{
  Serial.begin(9600); // Serial for debugging

  init();
  if (MASTER)
  {
    myIR.IRinit(38, 12);
  }
  else
  {
    myIR.IRinit(56, 12);
  }
  link myLink(&myIR, MASTER); //ir obj, master/slave, broadcast interval (miliseconds)

  nunchuk.init();

  // Generate game colour
  GameColour gameColour;

  // Start screen and make it the right colour
  Screen.begin();
  Screen.setRotation(2);
  Screen.fillScreen(gameColour.getGameColour());

  Gfx gfx(&Screen);
  Scoreboard scoreboard = Scoreboard(&gfx, &Screen);

  // Create TaskManager object
  TaskManager *taskManager = new TaskManager;

  // Create Task objects
  IntroScreen *introScreen = new IntroScreen(&Screen, &Touch, &nunchuk, &gameColour);
  SettingMenu *settingMenu = new SettingMenu(&Screen, &Touch, &nunchuk, &gameColour);
  
  Game game(15, 17, &Screen, &gfx, &scoreboard, &myIR);

  Control *control = new Control(&nunchuk, &Screen, &gfx, &scoreboard, &game);

  // Add tasks to taskManager
  taskManager->addTask(introScreen);
  taskManager->addTask(settingMenu);
  taskManager->addTask(control);

  Serial.begin(9600); // Serial for debugging
  Serial.println("Start");

  // Start touch screen
  if (!Touch.begin())
  {
    Serial.println("TOUCH SCREEN NOT FOUND");
    while (1)
      ; // Touch screen doesn't work!
  }

  // Loop
  while (1)
  {
    settingMenu->updateScreenBrightness(); // update screen brightness
    taskManager->doTask();                 // do current task
    //taskManager->doTask();                 // do current task

    if (myIR.getTime_ms() > update_timer)
    { //do this every 1000ms
      freeRam();
      update_timer = (myIR.getTime_ms() + 500);
    }
  }
}
