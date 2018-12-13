#ifndef settingMenu_H
#define settingMenu_H

class settingMenu
{
  public:
    settingMenu(touchScreen *ts);
    uint8_t run(void);
    void screenBrightness(void);
    
  private:

    void PWM_init(void);
    void adc_init(void);
    uint8_t screenbrightnessAuto(void);

    uint8_t photocellReading;
    uint8_t brightness = 150;
    uint8_t brightness_old = 150;

    touchScreen *ts;
    Adafruit_STMPE610 *touch;
    Adafruit_ILI9341 *tft;
};

#endif
