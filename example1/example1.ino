#include "IR.h"

char receivedChars[8];   // an array to store the received data

IR myIR(38); //38KHz sender

int main(void) {
  Serial.begin(9600);
  
  while(1) {

    if(IR.available){
      printArray(IR.read);
    }

    if(IR.error){
      Serial.print("Something went wrong);
    }
    
    recvWithEndMarker(); 
    
    if (newData == true) {
      IR.write(receivedChars);
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
      if (ndx >= 8) {
        ndx = 8 - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void printArray(uint8_t massage[8]) {
  uint8_t x = 0;
  while (massage[x] > 0) {
    Serial.print(massage[x]);
    x++;
  }
  Serial.println("");
}
