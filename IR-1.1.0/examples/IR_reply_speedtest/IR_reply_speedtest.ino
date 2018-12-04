/*
 * This example is made by Christiaan van den Berg.
 * Its a simple program to test the maximum trans/rec speed
 */

#include "IR.h"

#define TRANSMITTER_FREQ 38
#define TIMER_ADD_VAL_38 0.02631579 
#define TIMER_ADD_VAL_56 0.01785714

char receivedChars[MESSAGE_SIZE];   // an array to store the received data
boolean newData = false;
int message = 0;
int messageSize = 0;
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
  Serial.println("Typ here your message to start the infinite loop...");
  
  while (1) {
    if (myIR.available()) {
      message++;
      char string1[MESSAGE_SIZE];
      myIR.read(string1);
      myIR.write(string1); //this is needed for the infinite loop
      Serial.println(string1);
    }
    if(ms_timer>1000){
      Serial.print("speed in B/s:");
      Serial.println((message*messageSize)/(ms_timer/1000));
      ms_timer = 0;
      message = 0;
    }
    if (myIR.error()) {
      Serial.print("Something went wrong");
    }
    recvWithEndMarker();
    if (newData == true) {
      myIR.write(receivedChars);
      newData = false;
    }
  }
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= 9) {
        ndx = 8;
      }
    }
    else {
      messageSize = ndx;
      Serial.println(messageSize);
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
