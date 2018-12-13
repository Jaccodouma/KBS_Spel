/*
   This example is made by Christiaan van den Berg.
   Its a simple program to test the maximum trans/rec speed
*/
#include "IR.h"
#include "link.h"

#define MASTER 1
#define TRANSMITTER_FREQ 38
#define TIMER_ADD_VAL_38 0.02631579
#define TIMER_ADD_VAL_56 0.01785714

float ms_timer = 0; //ms_timer die boven in de hoek van het scherm staat

IR myIR(TRANSMITTER_FREQ, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  ms_timer += TIMER_ADD_VAL_38;
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  Serial.begin(9600);
  Serial.println("start");

  link myLink(&myIR, MASTER);

int deleteme = 0;

  while (1) {

    if(deleteme<16){
      deleteme++;
    }else{
      deleteme = 0;
    }
    
    myLink.UpdatePlayerInfo(2,deleteme,0,0); //X,Y,BOMB,LIFES

    Serial.print(" ! x:");
    Serial.print(myLink.otherplayer_x);
    Serial.print("  y:");
    Serial.print(myLink.otherplayer_y);
    Serial.print("  bomb:");
    Serial.print(myLink.otherplayer_bomb);
    Serial.print("  lifes:");
    Serial.println(myLink.otherplayer_lifes);

    myLink.checkForData(); //keep repeating this in the loop to stay connected

_delay_ms(50);

  }
}
