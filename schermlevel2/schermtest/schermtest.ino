#include <avr/io.h>
#include "gfx.h"
#include "control.h"
#include "IR.h"

#define TRANSMITTER_FREQ 38
#define TIMER_ADD_VAL_38 0.02631579
#define TIMER_ADD_VAL_56 0.01785714

float ms_timer = 0; //ms_timer use this timer instead of delays

IR myIR(TRANSMITTER_FREQ, 12); //38KHz or 56KHz Transmitter Fq, pulse size (45 default)

ISR(TIMER2_OVF_vect)
{
  ms_timer += TIMER_ADD_VAL_38;
  myIR.timerOverflow();
}

ISR(PCINT2_vect)
{
  myIR.pinChange();
}

void initNunchukPins()
{
  // nunchuk
  DDRC |= (1 << DDC2) | (1 << DDC3); // Set PC2 & PC3 on OUTPUT
  PORTC &= ~(1 << PORTC2);           // set PC2 to LOW
  PORTC |= (1 << PORTC3);            // Set PC3 to HIGH
}

int main(void)
{
  init();
  Serial.begin(9600);
  initNunchukPins();

  Gfx gfx;
  Control control(&gfx);
  control.startGame();

  myIR.IRinit(TRANSMITTER_FREQ, 12);

  while (1)
  {
    if (myIR.available())
    {
      char string1[MESSAGE_SIZE];
      myIR.read(string1);
      Serial.println(string1);
    }

    if (myIR.error())
    {
      Serial.print("Something went wrong while receiving");
    }

    if (ms_timer > 5)
    {
      //Serial.print(ms_timer);
      // Serial.print(" - ");
      control.update();
      sentPlayerInfo(control.game->player->getFieldPos().x,control.game->player->getFieldPos().y);
      ms_timer = 0;
    }
  }
}

void sentPlayerInfo(uint8_t x, uint8_t y) {
  uint8_t message[2];
  Serial.print(x-1, DEC);
  Serial.print(" - ");
  Serial.print(y-1, DEC);
  Serial.print(" - ");

  message[0] = ((y-1)<<7);
  message[1] = ((x-1)<<4) | ((y-1)>>1);
  
  for(uint8_t n = 0; n < 8; n++){
    if(message[0] & (1<<n)){
      Serial.print("1");
    }else{
      Serial.print("0");
    }
  }
  Serial.print(" ");
    for(uint8_t n = 0; n < 8; n++){
    if(message[1] & (1<<n)){
      Serial.print("1");
    }else{
      Serial.print("0");
    }
  }
  Serial.println();

  myIR.read(message);
}
