/*
   This example is made by Christiaan van den Berg.
   Its a simple program to test the maximum trans/rec speed
*/
#include "IR.h"
#include "link.h"

#define MASTER 1
#define TRANSMITTER_FREQ 38

unsigned long speed_timer;
int deleteme = 1;

IR myIR(TRANSMITTER_FREQ, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  init();
  myIR.IRinit(TRANSMITTER_FREQ, 12);

  Serial.begin(9600);
  Serial.println("start");
  link myLink(&myIR, MASTER, 1000); //ir obj, master/slave, broadcast interval (miliseconds)

  uint16_t kleur = 12348;
  if (myLink.updateColorData(kleur)) {
    Serial.println("color add to buffer");
  }
//  if (myLink.updatePlayerData(1, 1, 1, 1)) { //X,Y,BOMB,LIVES
 //   Serial.println("player add to buffer");
 // }

  while (1) {

    if ((myIR.getTime_ms() % 8000) == 0) { //do this every 1000ms
       if (myLink.updatePlayerData(deleteme, 1, 1, 1)) { //X,Y,BOMB,LIVES
         Serial.println("player add to buffer");
       }
       deleteme++;
       if(deleteme>10){
        deleteme = 1;
       }
    }


    if ((myIR.getTime_ms() % 1000000) == 0) { //do this every 5000ms
      // uint16_t kleur = 12345;
      // if (myLink.updateColorData(kleur)) {
      //  Serial.println("color add to buffer");
      // }
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
      speed_timer = (myIR.getTime_ms() + 5000);
    }
  }
}
