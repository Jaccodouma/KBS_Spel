

#include "IR.h"

char receivedChars[MESSAGE_SIZE];   // an array to store the received data
boolean newData = false;
int message = 0;
int messageSize = 0;
unsigned long speed_timer;

IR myIR(38, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  init();
  myIR.IRinit(38, 12);
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

    if (myIR.getTime_ms() > speed_timer) { //do this every 2000ms
      Serial.print("                                speed:");
      Serial.println(myIR.getByteRate());
      speed_timer = (myIR.getTime_ms() + 1000);
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
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
