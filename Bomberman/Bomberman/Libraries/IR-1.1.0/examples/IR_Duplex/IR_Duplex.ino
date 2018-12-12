

#include "IR.h"

char receivedChars[MESSAGE_SIZE];   // an array to store the received data
boolean newData = false;
int message = 0;
int messageSize = 0;
float timer = 0; //timer die boven in de hoek van het scherm staat

IR myIR(56, 13); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER0_OVF_vect)  //begin met je taken als timer2 een overflow heeft
{
  timer += 0.016384; //T= maxrigistersize*(1/F_CPU)*1024 = 256*(1/16.000.0000)*1024 = 0.016384 s/tick
}

ISR(TIMER2_OVF_vect) {
  myIR.timerOverflow();
}

ISR(PCINT2_vect) {
  myIR.pinChange();
}

void timer0Init(){
  cli(); //negeer alle interupts
  TCCR0A = (1<< COM0B1); //zet alle bits op 0
  TCCR0B |= (1 << WGM01); // zet timer 2 op CTC mode
  TCCR0B |= (1 << CS00) | (1 << CS01) | (1 << CS02); //klok 2 is nu geprescalled op 1024
  //T= maxrigistersize*(1/F_CPU)*1024 = 256*(1/16.000.0000)*1024 = 0.016384 s/tick
  TIMSK0 |= (1 << TOIE0); //geef een interrupt als er een overflow is
  sei(); //reageer op alle interupts
}

int main(void) {
  timer0Init();
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

    if(timer>1){
      Serial.print("speed in B/s:");
      Serial.println((message*messageSize)/timer);
      timer = 0;
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
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
