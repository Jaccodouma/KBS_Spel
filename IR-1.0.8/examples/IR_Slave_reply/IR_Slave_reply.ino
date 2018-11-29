

#include "IR.h"

char receivedChars[MESSAGE_SIZE];   // an array to store the received data

boolean newData = false;

IR myIR(38, 100); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

int main(void) {
  Serial.begin(9600);

  while (1) {

    if (myIR.available()) {
      char string1[MESSAGE_SIZE];
      myIR.read(string1);
      myIR.write(string1);
      printArray(string1);
    }

    if (myIR.error()) {
      Serial.print("Something went wrong");
    }
  }
}

void printArray(uint8_t message[8]) {
  uint8_t x = 0;
  while (message[x] > 0) {
    Serial.print((char)message[x]);
    x++;
  }
  Serial.println("");
}
