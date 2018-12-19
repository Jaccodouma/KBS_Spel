#ifndef link_H
#define link_H

#define MESSAGE_SIZE 9

class link
{
  public:
    link(IR *ir, uint8_t master); //constructor
    
    uint8_t checkForData(void); //check if there is new player data
    
    void UpdatePlayerInfo(uint8_t x, uint8_t y, uint8_t bomb, uint8_t lives); //update the player data for the other defice
    
    void updateTimer(void); //update the timer

    uint8_t ping();

    uint8_t otherplayer_x, otherplayer_y, otherplayer_bomb, otherplayer_lives; //Public variables to update the "the going to be sent" data

  private:
    IR *ir;
    uint8_t master;
    uint8_t Enableping = 1;
    uint8_t readyForNewData = 1;
    uint8_t resentCmd[1] = {(1 << 1)};
    uint8_t EnableSending = 1;
    uint8_t dataIN[MESSAGE_SIZE] = {0, 0}; // an array to store the received data
    uint8_t dataOUT[MESSAGE_SIZE] = {0, 0};
};

#endif
