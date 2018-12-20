#ifndef link_H
#define link_H

#define MESSAGE_SIZE 4

#define COLOR_DATA 2
#define PLAYER_DATA 1

#define REGUEST_SENT_AGAIN_CMD 0b00010001       //tell the other to sent the data previous again again
#define ACKNOWLEDGE_CORRECT_DATA_CMD 0b00100001 //acknowlegdgement to tell the othersize that the data is reveived

class link
{
  public:
    link(IR *ir, uint8_t master, uint16_t broadcastInterval); //constructor

    //communcation funtions
    uint8_t checkForData(void); //Manage the communication
    uint8_t updatePlayerData(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives); //add the player data for the other defice to the buffer (if possible)
    uint8_t updateColorData(uint16_t color); //add the color data for the other defice to the buffer (if possible)

    //Public variables to check the other player
    uint8_t otherplayer_x, otherplayer_y, otherplayer_bomb, otherplayer_lives = 0;
    uint16_t otherplayer_color = 0;

  private:
    IR *ir;
    uint8_t master;
    uint8_t broadcastInterval = 0;
    unsigned long broadcast_timer = 0;
    
    uint8_t dataIN[MESSAGE_SIZE]; //an array to store the received data

    void newBroadcast(void); //message manager
    uint8_t lastSent = 0; //stores the last datatype; 0 = nothing, 1 = colordata, 2 = playerdata

    //variables to store the new data
    uint8_t newPlayerdata = 0;
    uint8_t newColordata = 0;
    uint8_t PlayerdataOUT[2]; //variable to store the player data
    uint8_t ColordataOUT[3];  //variable to store the color data
};

#endif
