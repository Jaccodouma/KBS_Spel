/*
   This example is made by Christiaan van den Berg.
*/
#include "IR.h"
#include "link.h"

#define MASTER 1

unsigned long speed_timer;
unsigned long update_timer;
unsigned long p_update_timer;

uint16_t kleur = 12348;

int deleteme = 1;

IR myIR(38, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  init();
  if (MASTER) {
    myIR.IRinit(38, 12);
  } else {
    myIR.IRinit(56, 12);
  }
  link myLink(&myIR, MASTER); //ir obj, master/slave, broadcast interval (miliseconds)

  Serial.begin(9600);
  Serial.println("start");

  while (1) {

    if (myIR.getTime_ms() > update_timer) { //do this every 1000ms
      if (myLink.updateColorData(kleur)) {
        Serial.println("color add to buffer");
      }
      kleur++;
      update_timer = (myIR.getTime_ms() + 5000);
    }

    if (myIR.getTime_ms() > p_update_timer) { //do this every 1000ms
      myLink.updatePlayerData(deleteme, 1, 1, 1); //X,Y,BOMB,LIVES
      deleteme++;
      if (deleteme > 10) {
        deleteme = 1;
      }
      p_update_timer = (myIR.getTime_ms() + 100);
    }
    
    if (myLink.checkForData() > 0) { //keep repeating this in the loop to stay connected
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

    if (myIR.getTime_ms() > speed_timer) { //do this every 2000ms
      Serial.print("                                speed:");
      Serial.println(myIR.getByteRate());
      speed_timer = (myIR.getTime_ms() + 2000);
    }
  }
}
