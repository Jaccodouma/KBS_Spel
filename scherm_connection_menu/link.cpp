#include "IR.h"

#include "link.h"

link::link(IR *ir, uint8_t master) {
  this->ir = ir;
  this->master = master;
}

uint8_t link::checkForData(void) {
  if (master && EnableSending) {
    Serial.print("sent data..");
    ir->write(dataOUT); //the master determines when the pransmission is being started
    EnableSending = 0;
    return 0;
  }

  if (ir->error()) {
    Serial.print("Something went wrong sent -resent-");
    ir->write(resentCmd); //sent a resent command to let the other side know that they need to resent.
    return 0;
  }

  if (ir->available()) { //there is some new valid data
    ir->read(dataIN);

    if (dataIN[0] & 0b00000001) { //check for ping
      ir->write(dataIN);      //just resent "ping command" back
      Serial.println("PING");
      return 0;
    }

    if ((dataIN[0] & 0b00000010) >> 1) { //check for resent
      Serial.println("error! we need to resent");
      EnableSending = 1;
      return 0;
    }
    otherplayer_x = ((dataIN[0] & 0b00111100) >> 2) + 1;
    otherplayer_y = (((dataIN[0] & 0b11000000) >> 6) | ((dataIN[1] & 0b00000111) << 2)) + 1;
    otherplayer_bomb = ((dataIN[1] & 0b00001000) >> 3);
    otherplayer_lifes = ((dataIN[1] & 0b00110000) >> 4);
    readyForNewData = 1;

    if (!master) { //slaves sent there data right after the new data is received
      ir->write(dataOUT);
    }
    //EnableSending = 1;
    return 1;
  }
}

void link::UpdatePlayerInfo(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lifes) {
  if (x < 16 && y < 32 && bomb <= 1 && lifes <= 3 && readyForNewData) {

    dataOUT[0] = (0 << 0) | (0 << 1) | ((x - 1) << 2) | ((y - 1) << 6);  //YYXX.XXRP
    dataOUT[1] = ((y - 1) >> 2) | (bomb << 3) | (lifes << 4);              //--LL.BYYY

   /* otherplayer_x = ((dataOUT[0] & 0b00111100) >> 2) + 1;
   // otherplayer_y = (((dataOUT[0] & 0b11000000) >> 6) | ((dataOUT[1] & 0b00000111) << 2)) + 1;
   // otherplayer_bomb = ((dataOUT[1] & 0b00001000) >> 3);
    //otherplayer_lifes = ((dataOUT[1] & 0b00110000) >> 4);

    Serial.print("ping:");
    Serial.print((dataOUT[0] & 0b00000001));
    Serial.print("  resent:");
    Serial.print(((dataOUT[0] & 0b00000010) >> 1));
    Serial.print("  x:");
    Serial.print(otherplayer_x);
    Serial.print("  y:");
    Serial.print(otherplayer_y);
    Serial.print("  bomb:");
    Serial.print(otherplayer_bomb);
    Serial.print("  lifes:");
    Serial.println(otherplayer_lifes);
    readyForNewData = 0;
    */
  } else {
    //Serial.println("wrong input or the message insnt receievd yet");
  }
}
