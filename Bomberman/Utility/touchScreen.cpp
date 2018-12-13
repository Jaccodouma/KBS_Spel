#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#include "touchScreen.h"

touchScreen::touchScreen(Adafruit_ILI9341 *tft, Adafruit_STMPE610 *touch) {
	this->touch = touch;
	this->tft = tft;
	if (!touch->begin()) {
		Serial.println("STMPE not found!");
		while (1);
	}
}

uint8_t touchScreen::checkmenuButton() {
	touchScreen::updateTS();
	tft->drawRect(224, 0, 16, 16, ILI9341_WHITE);

	if (x > STMP_MENU_Xmin && x < STMP_MENU_Xmax && y > STMP_MENU_Ymin && y < STMP_MENU_Ymax) {
		if (check) {
			check = 0;
			return 1;
			} else {
			return 0;
		}
		} else {
		check = 1;
		return 0;
	}
}

uint8_t touchScreen::checkmenuCheckBox1(uint8_t EnablePrint) {
	touchScreen::updateTS();

	if (EnablePrint) {
		tft->setCursor(100, 8);
		tft->setTextColor(ILI9341_WHITE);
		tft->setTextSize(1);
		tft->println("AutoMode:");
	}

	if (x > STMP_CheckBox1_Xmin && x < STMP_CheckBox1_Xmax && y > STMP_CheckBox1_Ymin && y < STMP_CheckBox1_Ymax) {
		if (check1) {
			checkboxbool1 ^= 1;
			check1 = 0;
		}
		} else {
		check1 = 1;
	}
	if (checkboxbool1) {
		if (EnablePrint) {
			tft->drawRect(150, 0, 16, 16, ILI9341_RED);
		}
		return 1;
		} else {
		if (EnablePrint) {
			tft->drawRect(150, 0, 16, 16, ILI9341_WHITE);
		}
		return 0;
	}
}

uint8_t touchScreen::checkScreenB(uint8_t input) {
	if (input > 5) {
		brightness = input;
		} else {
		touchScreen::updateTS();
		if (x > STMP_ScreenB_Xmin && x < STMP_ScreenB_Xmax && y > STMP_ScreenB_Ymin && y < STMP_ScreenB_Ymax) {
			brightness = map(y, STMP_ScreenB_Ymin, STMP_ScreenB_Ymax, 5, 255);
		}
	}

	if (brightness > 0 && brightness != brightness_old) {
		tft->drawRect(201, map(brightness_old, 5, 255, 172, 30), 30, 8, ILI9341_BLUE);
		brightness_old = brightness;
	}
	tft->drawRect(200, 30, 32, 150, ILI9341_WHITE);
	tft->drawRect(200, map(brightness, 5, 255, 172, 30), 32, 8, ILI9341_WHITE);
	return brightness;
}

void touchScreen::printXY() {
	touchScreen::updateTS();
	Serial.print(x);
	Serial.print(" - ");
	Serial.println(y);
}

void touchScreen::updateTS() {
	if (touch->touched()) {
		while (!touch->bufferEmpty()) {
			touch->readData(&x, &y, &z);
		}
		touch->writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
	}
}
