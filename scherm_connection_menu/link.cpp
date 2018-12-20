#include "IR.h"
#include "link.h"

/*  explaination of the sent cmds

    NOTES:
     -  if a byte is '0' the byte isn't sent
     -  All messages have a full validation check.
     -  Some messages also have a acknowledgment.

    Y = y
    X = x
    B = bomb
    L = lives
    H = hash (based on parity)
    C = color

   ----(WITHOUT ACK)----
    player data (in game)
       byte 1 |  byte 2 | byte 3
     1XXX.XHHH|YYYY.YBLL|0000.0000
   ---------------------------

   -----(WITH ACK)------
    sent "i received a invalid message" (sent agian) cmd
       byte 1 |  byte 2 | byte 3
     0001.0HHH|0000.0000|0000.0000
   ----------------------------
    sent "i received a valid message" cmd
       byte 1 |  byte 2 | byte 3
     0010.0HHH|0000.0000|0000.0000
   ----------------------------
    color
       byte 1 |  byte 2 | byte 3
     0011.0HHH|CCCC.CCCC|CCCC.CCCC
   ----------------------------
    not used yet...
       byte 1 |  byte 2 | byte 3
     0100.0HHH|0000.0000|0000.0000
   ----------------------------
*/

link::link(IR *ir, uint8_t master, uint16_t broadcastInterval) {
  this->ir = ir;
  this->master = master;
  this->broadcastInterval = broadcastInterval;
}

uint8_t link::checkForData(void) {
  if (ir->getTime_ms() > broadcast_timer) { //the master will broadcast every "broadcastInterval" (miliseconds)
    if (master) {
      link::newBroadcast(); //the master determines when the transmission is being started
    }
    broadcast_timer = (ir->getTime_ms() + broadcastInterval);
  }

  if (ir->error()) {
    Serial.print("par error");
    uint8_t Tempdata[] = {REGUEST_SENT_AGAIN_CMD};
    ir->write(Tempdata[0]);
    return 0;
  }

  if (ir->available()) { //there is some new valid data
    ir->read(dataIN);
    //we need to double check the data with a hash (3 parity bits. but we call it a hash)
    uint8_t tempHash = (dataIN[0] & 0b00000111); //store the dataIN hash
    dataIN[0] &= ~0b00000111; //set the "hash position" to zero (otherwise the hash is calculated with the hash in it..)
    uint8_t temphashOUT = 0; //variable to store the to be calculated hash

    for (uint8_t arrayN = 0; arrayN < MESSAGE_SIZE; arrayN++) {
      for (uint8_t bitN = 0; bitN < 8; bitN++) {
        if (dataIN[arrayN] & (1 << bitN)) {
          temphashOUT++;
        }
      }
    }
    temphashOUT = (temphashOUT % 8); //make a 3 bit value
    if (temphashOUT != tempHash) { //compare the hash
      //if the hash is incorrect
      Serial.println("hash error! we need to sent again");
      uint8_t Tempdata[] = {REGUEST_SENT_AGAIN_CMD};
      ir->write(Tempdata[0]);
      return 0;
    } else {
      //correct hash, now we need to sort the data

      if ((dataIN[0] & 0b10000000) == 0b10000000) { //check for player data (without ack)
        Serial.println("player data");
        otherplayer_x = ((dataIN[0] & 0b01111000) >> 3) + 1;
        otherplayer_y = ((dataIN[1] & 0b11111000) >> 3) + 1;
        otherplayer_bomb = ((dataIN[1] & 0b00000100) >> 2);
        otherplayer_lives = (dataIN[1] & 0b00000011);
      }

      if ((dataIN[0] & 0b11110000) == 0b00010000) { //check for an invalid data message
        Serial.println("invalid data");
        link::newBroadcast(); //sent the data again
        return 0;
      }

      if ((dataIN[0] & 0b11110000) == 0b00100000) { //check for valid data message
        Serial.println("succes! valid data");
        if (lastSent = COLOR_DATA) {
          newColordata = 0; //color data can be updated
        }
      }

      if ((dataIN[0] & 0b11110000) == 0b00110000) { //check for color data
        Serial.println("color data");
        otherplayer_color = dataIN[1] | (dataIN[1] << 8);
        Serial.println("sent an ack");
        uint8_t Tempdata[] = {ACKNOWLEDGE_CORRECT_DATA_CMD};
        ir->write(Tempdata[0]); //we tell the other defice that we correctly reveived the message
      }
      if (!master) { //slaves always sent there data right after the new data is received
        link::newBroadcast();
      }
      return 1;
    }
  }
}

void link::newBroadcast(void) {
  /*  priority list;
      1.  color data
      2.  player data
  */
  if (newColordata) {
    ir->write(ColordataOUT);
    lastSent = COLOR_DATA;
  } else if (newPlayerdata) {
    ir->write(PlayerdataOUT);
    lastSent = PLAYER_DATA;
    newPlayerdata = 0; //player data can be updated
  } else {
    //nothing because there isn't new data yet
  }
}


uint8_t link::updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives) {
  if (x <= 15 && x > 0 && y <= 17 && y > 0 && bomb <= 1 && lives <= 3 && !newPlayerdata) { //!!!!! check if there is space in the buffer
    /* ----------------------------
       player data (in game)
          byte 1 |  byte 2 | byte 3
        1XXX.XHHH|YYYY.YBLL|0000.0000
      ----------------------------
    */
    PlayerdataOUT[0] = (1 << 7) | ((x - 1) << 3);
    PlayerdataOUT[1] = ((y - 1) << 3) | (bomb << 2) | (lives << 0);

    uint8_t hashOUT = 0;
    for (uint8_t arrayN = 0; arrayN <= 2; arrayN++) {
      for (uint8_t bitN = 0; bitN < 8; bitN++) {
        if (PlayerdataOUT[arrayN] & (1 << bitN)) {
          hashOUT++;
        }
      }
    }
    hashOUT = (hashOUT % 8);
    PlayerdataOUT[0] |= hashOUT; //1XXX.XHHH
    newPlayerdata = 1;
    return 1;
  }
  Serial.println("error player data is already filled");
  return 0;
}

uint8_t link::updateColorData(uint16_t color) {
  if (color > 0 && !newColordata) { //!!!!! check if there is space in the buffer
    /*color
        byte 1 |  byte 2 | byte 3
      0011.0HHH|CCCC.CCCC|CCCC.CCCC
    */
    ColordataOUT[0] = 0b00110000;
    ColordataOUT[1] = (color >> 8);
    ColordataOUT[2] = color;

    uint8_t hashOUT = 0;
    for (uint8_t arrayN = 0; arrayN < MESSAGE_SIZE; arrayN++) {
      for (uint8_t bitN = 0; bitN < 8; bitN++) {
        if (ColordataOUT[arrayN] & (1 << bitN)) {
          hashOUT++;
        }
      }
    }
    hashOUT = (hashOUT % 8);
    ColordataOUT[0] |= hashOUT; //1XXX.XHHH
    newColordata = 1;
    return 1;
  }
  Serial.println("error color data is already filled");
  return 0;
}
