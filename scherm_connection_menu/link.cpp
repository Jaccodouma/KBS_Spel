#include "IR.h"

#include "link.h"

link::link(IR *ir, uint8_t master) {
  this->ir = ir;
  this->master = master;
}

uint8_t link::ping() {
  Enableping = 0; //to prevent a infinite loop
  dataOUT[0] = 1;
  dataOUT[1] = 0;
  ir->write(dataOUT); //sent a ping signal

  if (ir->available()) { //there is some new valid data
    ir->read(dataIN);
    if (dataIN[0] & 0b00000001) { //check for ping
      ir->write(dataIN);      //just resent "ping command" back
      Serial.println("PING");
      return 1;
    }
  }
  return 0;
}

uint8_t link::checkForData(void) {
  Enableping = 1;
  if (master && EnableSending) {
    ir->write(dataOUT); //the master determines when the pransmission is being started
    EnableSending = 0;
    return 0;
  }

  if (ir->error()) {
    Serial.print("par error");
    ir->write(resentCmd); //sent a resent command to let the other side know that they need to resent.
    return 0;
  }

  if (ir->available()) { //there is some new valid data
    ir->read(dataIN);

    if (dataIN[0] & 0b00000001 && Enableping) { //check for ping
      ir->write(dataIN);      //just resent "ping command" back
      Serial.println("PING");
      return 0;
    }

    if ((dataIN[0] & 0b00000010) >> 1) { //check for resent
      Serial.println("par error! we need to sent again");
      EnableSending = 1;
      return 0;
    } else { //the parity is correct, now we need to check the hash
      //calculate the hash
      uint8_t hashIN = 0;

      for (uint8_t bN = 0; bN < 8; bN++) {
        if (dataIN[0] & (1 << bN)) {
          hashIN++;
        }
      }
      for (uint8_t bN = 0; bN < 6; bN++) {
        if (dataIN[1] & (1 << bN)) {
          hashIN++;
        }
      }

      hashIN = (hashIN % 4);
      if (hashIN != ((dataIN[1] & 0b11000000) >> 6)) { //compare the hash
        //if the hash is incorrect
        Serial.println("hash error! we need to sent again");
        Serial.print("hash:");
        Serial.println(hashIN);
        EnableSending = 1;
        return 0;
      } else {
        //calculate the other values
        otherplayer_x = ((dataIN[0] & 0b00111100) >> 2) + 1;
        otherplayer_y = (((dataIN[0] & 0b11000000) >> 6) | ((dataIN[1] & 0b00000111) << 2)) + 1;
        otherplayer_bomb = ((dataIN[1] & 0b00001000) >> 3);
        otherplayer_lives = ((dataIN[1] & 0b00110000) >> 4);

        if (!master) { //slaves sent there data right after the new data is received
          ir->write(dataOUT);
        }
        EnableSending = 0;
        return 1;
      }
    }
  }
  return 0;
}

void link::UpdatePlayerInfo(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives) {
  if (x <= 15 && x > 0 && y <= 17 && y > 0 && bomb <= 1 && lives <= 3 && readyForNewData) {

    dataOUT[0] = (0 << 0) | (0 << 1) | ((x - 1) << 2) | ((y - 1) << 6);             //YYXX.XXRP
    dataOUT[1] = ((y - 1) >> 2) | (bomb << 3) | (lives << 4) | (0 << 6) | (0 << 7); //--LL.BYYY

    uint8_t hashOUT = 0;
    for (uint8_t aN = 0; aN < 2; aN++) {
      for (uint8_t bN = 0; bN < 8; bN++) {
        if (dataOUT[aN] & (1 << bN)) {
          hashOUT++;
        }
      }
    }
    hashOUT = (hashOUT % 4);

    dataOUT[1] |= (hashOUT << 6); //HHLL.BYYY
    //Serial.print("hash:");
    //Serial.println(hashOUT);

    if (master) {
      ir->write(dataOUT); //the master determines when the pransmission is being started
    }
    EnableSending = 0;

  } else {
    Serial.println("error");
  }
}
