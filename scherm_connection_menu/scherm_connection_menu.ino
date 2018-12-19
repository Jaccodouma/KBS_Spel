/*
   This example is made by Christiaan van den Berg.
   Its a simple program to test the maximum trans/rec speed
*/
#include "IR.h"
#include "link.h"

#define MASTER 1
#define TRANSMITTER_FREQ 56

unsigned long sent_timer;
unsigned long speed_timer;
unsigned long ping_timer;
uint8_t deleteme = 0;

IR myIR(TRANSMITTER_FREQ, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  Serial.begin(9600);
  Serial.println("start");
  link myLink(&myIR, MASTER);

  while (!myLink.ping()) {
    Serial.println("wait for reply");
  }

  while (1) {
    if (myIR.getTime_ms() > sent_timer) { //do this every 100ms
      myLink.UpdatePlayerInfo(deleteme, 2, 0, 1); //X,Y,BOMB,LIVES
      sent_timer = (myIR.getTime_ms() + 100);
      deleteme++;
      if (deleteme > 10) {
        deleteme = 1;
      }
    }

    if (myIR.getTime_ms() > speed_timer) { //do this every 2000ms
      Serial.print("                                speed:");
      Serial.println(myIR.getByteRate());
      speed_timer = (myIR.getTime_ms() + 2000);
    }

    if (myLink.checkForData()) { //keep repeating this in the loop to stay connected
      Serial.print(" ! x:");
      Serial.print(myLink.otherplayer_x);
      Serial.print("  y:");
      Serial.print(myLink.otherplayer_y);
      Serial.print("  bomb:");
      Serial.print(myLink.otherplayer_bomb);
      Serial.print("  lives:");
      Serial.println(myLink.otherplayer_lives);
    }
  }
}
