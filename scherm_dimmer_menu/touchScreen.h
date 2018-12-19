#ifndef touchScreen_H
#define touchScreen_H

#define STMP_MENU_Xmin 180
#define STMP_MENU_Xmax 600
#define STMP_MENU_Ymin 3500
#define STMP_MENU_Ymax 3900

#define STMP_CheckBox1_Xmin 1300
#define STMP_CheckBox1_Xmax 1600
#define STMP_CheckBox1_Ymin 3500
#define STMP_CheckBox1_Ymax 3700

#define STMP_ScreenB_Xmin 180
#define STMP_ScreenB_Xmax 1000
#define STMP_ScreenB_Ymin 1700
#define STMP_ScreenB_Ymax 3400

class touchScreen
{
	public:
	touchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch);
	uint8_t checkmenuButton(void);
	uint8_t checkmenuCheckBox1(uint8_t EnablePrint);
	uint8_t checkScreenB(uint8_t input);
	void printXY(void);

	private:
	Adafruit_STMPE610 *touch;
	Adafruit_ILI9341 *tft;
	void updateTS();
	uint8_t checkboxbool1 = 1;
	uint8_t check = 1;
	uint8_t check1 = 1;
	uint16_t x;
	uint16_t y;
	uint8_t z;
	uint8_t brightness = 150;
	uint8_t brightness_old = 150;
};

#endif
