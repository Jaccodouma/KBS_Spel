#ifndef touchScreen_H
#define touchScreen_H

#define STMP_MENU_Xmin 180
#define STMP_MENU_Xmax 600
#define STMP_MENU_Ymin 3500
#define STMP_MENU_Ymax 3900

#define STMP_ScreenB_Xmin 180
#define STMP_ScreenB_Xmax 1000
#define STMP_ScreenB_Ymin 1700
#define STMP_ScreenB_Ymax 3400

class touchScreen
{
  public:
    touchScreen(Adafruit_STMPE610 *touch);
    uint8_t checkmenuButton(void);
    uint8_t checkScreenB(void);

  private:
    Adafruit_STMPE610 *touch;
    void updateTS();
    uint16_t x;
    uint16_t y;
    uint8_t z;
};

#endif
