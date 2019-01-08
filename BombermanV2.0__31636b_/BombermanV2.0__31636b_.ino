/*
    Name:       Bomberman.ino
    Created:	12/12/2018 12:02:28
    Author:     Jacco & Christiaan
*/

// Libraries for screen
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h" // TFT screen

// Other libraries
#include "SPI.h"
#include <Wire.h>
#include <stdlib.h> // for the random() function

// Self-made Libraries and utilities
#include "TaskManager.h"
#include "GameColour.h"
#include "Game.h"
#include "IR.h"
#include "link.h"

// Task classes
#include "IntroScreen.h"
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

unsigned long speed_timer;
unsigned long update_timer;
unsigned long p_update_timer;

uint16_t kleur = 12348;

int deleteme = 1;

int main(void)
{
  init();
  Serial.begin(115200); // Serial for debugging

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
  Game game(15, 17, &Screen, &gfx, &scoreboard, &myIR);

  // Create TaskManager object
  TaskManager *taskManager = new TaskManager;

  // Create Task objects
  IntroScreen *introScreen = new IntroScreen(&Screen, &nunchuk, &gameColour);
  Control *control = new Control(&nunchuk, &Screen, &gfx, &scoreboard, &game);

  // Add tasks to taskManager
  taskManager->addTask(introScreen);
  taskManager->addTask(control);

  Serial.println("STARTGAME");

  // Loop
  while (1)
  {
    taskManager->doTask(); // do current task

    if (myIR.getTime_ms() > update_timer)
    { //do this every 1000ms
      if (myLink.updateColorData(kleur))
      {
        Serial.println("color add to buffer");
      }
      kleur++;
      update_timer = (myIR.getTime_ms() + 5000);
    }

    if (myIR.getTime_ms() > p_update_timer)
    {                                             //do this every 1000ms
      myLink.updatePlayerData(deleteme, 1, 1, 1); //X,Y,BOMB,LIVES
      deleteme++;
      if (deleteme > 10)
      {
        deleteme = 1;
      }
      p_update_timer = (myIR.getTime_ms() + 200);
    }

    if (myLink.checkForData() > 0)
    { //keep repeating this in the loop to stay connected
      Serial.print(" x:");
      Serial.print(myLink.otherplayer_x);
      Serial.print("  y:");
      Serial.print(myLink.otherplayer_y);
      Serial.print("  bomb:");
      Serial.print(myLink.otherplayer_bomb);
      Serial.print("  lives:");
      Serial.print(myLink.otherplayer_lives);
      Serial.print("  color:");
      Serial.println(myLink.otherplayer_color);
    }

    if (myIR.getTime_ms() > speed_timer)
    { //do this every 2000ms
      Serial.print("                                speed:");
      Serial.println(myIR.getByteRate());
      freeRam();
      speed_timer = (myIR.getTime_ms() + 2000);
    }
  }
}
