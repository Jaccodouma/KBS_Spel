#ifndef link_H
#define link_H

#include "IR.h"

#define MESSAGE_SIZE 9

#define COLOR_DATA 2
#define PLAYER_DATA 1

#define WAIT_FOR_NEW_DATA 1
#define KEEP_SENDING_UNTIL_ACK 2
#define RECEIVE 3

#define DEBUG 0

class link
{
  public:
    link(IR *ir, uint8_t master); //constructor

    //communcation funtions
    uint8_t checkForData(void); //Manage the communication
    uint8_t updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives); //add the player data for the other defice to the buffer (if possible)
    uint8_t updateColorData(uint16_t color); //add the color data for the other defice to the buffer (if possible)

    //Public variables to check the other player
    uint8_t otherplayer_x = 0, otherplayer_y = 0, otherplayer_bomb = 0, otherplayer_lives = 0;
    uint16_t otherplayer_color = 0;
    boolean newPlayerdata = 0;
    boolean newColordata = 0;

  private:
    IR *ir;
    uint8_t master;
    uint8_t dataIN[MESSAGE_SIZE]; //an array to store the received data
    uint8_t lastSent = 0; //stores the last datatype; 0 = nothing, 1 = colordata, 2 = playerdata
    uint8_t PlayerdataOUT[2]; //variable to store the player data
    uint8_t ColordataOUT[3];  //variable to store the color data

    uint8_t calcHash(uint8_t data[MESSAGE_SIZE]);
    void broadcast(void); //message manager
};

#endif
