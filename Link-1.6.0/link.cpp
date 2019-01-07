#include "IR.h"
#include "link.h"

/*  explaination of the sent cmds
    NOTES:
     -  if a byte is '0' the byte isn't sent
    Y = y
    X = x
    B = bomb
    L = lives
    H = hash (based on parity)
    C = color
   ---------------------------
    player data (in game)
       byte 1 |  byte 2
     1XXX.XHHH|YYYY.YBLL
   ---------------------------
    sent "i received a invalid message" (sent agian) cmd
       byte 1
     0001.0HHH
   ----------------------------
    color
       byte 1 |  byte 2 | byte 3
     0011.0HHH|CCCC.CCCC|CCCC.CCCC
   ----------------------------
*/

link::link(IR *ir, uint8_t master) {
  this->ir = ir;
  this->master = master;
}

uint8_t link::checkForData(void) {

  if (ir->error()) {
    if (DEBUG) {
      Serial.println("message didn't arrive");
    }
    return 0;
  }

  if (ir->available()) { //there is some new valid data
    if ((ir->data[0] & 0b00000111) != link::calcHash(ir->data)) { //compare the hash
      //if the hash is incorrect
      if (DEBUG) {
        Serial.print("hash error!");
        Serial.print("calcH ");
        Serial.print(link::calcHash(ir->data));
        Serial.print(" - dataH ");
        Serial.println((ir->data[0] & 0b00000111));
      }
      if (!master) {
        link::broadcast();
      }
      return 0;
    } else {
      //correct hash, now we need to sort the data
      if ((ir->data[0] & 0b10000000) == 0b10000000) { //check for player data (without ack)
        if (DEBUG) {
          Serial.println("player data");
        }
        otherplayer_x = ((ir->data[0] & 0b01111000) >> 3) + 1;
        otherplayer_y = ((ir->data[1] & 0b11111000) >> 3) + 1;
        otherplayer_bomb = ((ir->data[1] & 0b00000100) >> 2);
        otherplayer_lives = (ir->data[1] & 0b00000011);
        if (!master) {
          link::broadcast();
        }
        return 1;
      } else {
        if ((ir->data[0] & 0b11110000) == 0b00110000) { //check for color data
          if (DEBUG) {
            Serial.println("color data");
          }
          otherplayer_color = ir->data[2] | (ir->data[1] << 8);
          if (!master) {
            link::broadcast();
          }
          return 1;
        }
      }
      if (!master) {
        link::broadcast();
      }
    }
  }
  return 0;
}

void link::broadcast(void) {
  /*  priority list;
      1.  color data
      2.  player data
  */
  if (newColordata) {
    ir->write(ColordataOUT);
    if (DEBUG) {
      Serial.println("c");
    }
    lastSent = COLOR_DATA;
    newColordata = 0;
  } else if (newPlayerdata) {
    ir->write(PlayerdataOUT);
    if (DEBUG) {
      Serial.println("p");
    }
    lastSent = PLAYER_DATA;
    newPlayerdata = 0;
  } else {
    lastSent = 0;
  }
}
/*
  void link::confirm(void) {
  if (lastSent == COLOR_DATA) {
    Serial.println("c data arrived");
    newColordata = 0;
  }
  if (lastSent == PLAYER_DATA) {
    Serial.println("p data arrived");
    newPlayerdata = 0; //player data can be updated
  }
  }
*/

uint8_t link::calcHash(uint8_t data[MESSAGE_SIZE]) {
  uint8_t hash = 0;
  for (uint8_t bitN = 3; bitN < 8; bitN++) {
    if (data[0] & (1 << bitN)) {
      hash++;
    }
  }
  uint8_t arrayN = 1;
  while (data[arrayN] > 0) {
    for (uint8_t bitN = 0; bitN < 8; bitN++) {
      if (data[arrayN] & (1 << bitN)) {
        hash++;
      }
    }
    arrayN++;
  }
  hash = (hash % 8);
  return hash;
}

uint8_t link::updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives) {
  if (x <= 15 && x > 0 && y <= 17 && y > 0 && bomb <= 1 && lives <= 3 && !newPlayerdata) { //!!!!! check if there is space in the buffer

    PlayerdataOUT[0] = (1 << 7) | ((x - 1) << 3);
    PlayerdataOUT[1] = ((y - 1) << 3) | (bomb << 2) | (lives << 0);

    PlayerdataOUT[0] |= link::calcHash(PlayerdataOUT); //1XXX.XHHH
    newPlayerdata = 1;
    if (DEBUG) {
      Serial.println("player add to buffer");
    }
  }
  if (master) {
    link::broadcast();
  }
  return 0;
}

uint8_t link::updateColorData(uint16_t color) {
  if (color > 0 && !newColordata) { //!!!!! check if there is space in the buffer

    ColordataOUT[0] = 0b00110000;
    ColordataOUT[1] = (color >> 8);     //most sig
    ColordataOUT[2] = ((color << 8) >> 8); //least sig

    ColordataOUT[0] |= link::calcHash(ColordataOUT); //1XXX.XHHH
    newColordata = 1;
    if (DEBUG) {
      Serial.println("color add to buffer");
    }
  }
  if (master) {
    link::broadcast();
  }
  return 0;
}
