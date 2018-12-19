#include <SPI.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "touchScreen.h"

#include "settingMenu.h"

SettingMenu::SettingMenu(touchScreen *ts) {
	this->ts = ts;
	SettingMenu::PWM_init();
	SettingMenu::adc_init();
	OCR0A = brightness;
}

int SettingMenu::run() {
	if (ts->checkmenuButton()) { //go back
		return 1;
	}
	if (ts->checkmenuCheckBox1(1)) {
		brightness = ts->checkScreenB(SettingMenu::screenbrightnessAuto());
		} else {
		brightness = ts->checkScreenB(0);
	}

	OCR0A = brightness;
	return 0;
}

void SettingMenu::screenBrightness(){
	if (ts->checkmenuCheckBox1(0)) {
		brightness = SettingMenu::screenbrightnessAuto();
	}
	OCR0A = brightness;
}

uint8_t SettingMenu::screenbrightnessAuto() {
	ADCSRA |= (1 << ADSC);        //single AD-conversion
	while (ADCSRA & (1 << ADSC)); //wait for result..
	photocellReading = ((ADC / 4) - 5);
	if (!((photocellReading < (brightness + 5)) && (photocellReading > (brightness - 5)))) {
		if (brightness < photocellReading) {
			if (brightness > 0) {
				brightness++;
			}
			} else {
			if (brightness < 255) {
				brightness--;
			}
		}
	}
	return brightness;
}

void SettingMenu::PWM_init()
{
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS02); //set clock prescaler 1024 and PWM, Phase and Frequency Correct
	DDRD = (1 << DDD6); //set OC0A as output PORTD6 pin6
}

void SettingMenu::adc_init()
{
	ADMUX = (1 << 0) | (1 << REFS0); //internal 5v reference and A1 as input
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //ADCclock=CPUclock/128
	ADCSRA |= (1 << ADEN);  //ADC enabled
	DDRC &= ~(1 << DDC1); // set A1 as input
	PORTC &= ~(1 << PIN1); // A1 pulldown off
}
