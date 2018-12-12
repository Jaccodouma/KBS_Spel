#include <avr/io.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_GFX.h>
#include "touchScreen.h"

#define TFT_CS 10
#define TFT_DC 9

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_STMPE610 touch = Adafruit_STMPE610(8);

uint8_t photocellReading;
uint8_t brightness = 150;
uint8_t brightness_old = 150;

void PWM_init(void)
{
  TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
  TCCR0B |= (1 << CS02); //set clock prescaler 1024 and PWM, Phase and Frequency Correct
  DDRD |= (1 << DDD6); //set OC0A as output PORTD6 pin6
}

void adc_init(void)
{
  ADMUX = (1 << 0) | (1 << REFS0); //internal 5v reference and A1 as input
  ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //ADCclock=CPUclock/128
  ADCSRA |= (1 << ADEN);  //ADC enabled
  DDRC &= ~(1 << DDC1); // set A1 as input
  PORTC &= ~(1 << PIN1); // A1 pulldown off
}

int main(void)
{
  PWM_init();
  adc_init();
  OCR0A = brightness;
  touchScreen myTS(&touch);
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLUE);

  while (1)
  {
    tft.drawRect(224, 0, 16, 16, ILI9341_WHITE);
    if (myTS.checkmenuButton()) {
      Serial.println("ja");
    }

    // screenbrightnessAuto();
    screenbrightnessMenu(&myTS);
    
  }
}

void screenbrightnessMenu(touchScreen *myTS)
{ 
  tft.drawRect(200, 30, 32, 150, ILI9341_WHITE);
  
  if (myTS->checkScreenB() > 0 && myTS->checkScreenB() != brightness_old) {
    brightness = myTS->checkScreenB();
    tft.drawRect(200, map(brightness_old, 5, 255, 172, 30), 32, 8, ILI9341_BLUE);
    tft.drawRect(200, map(brightness, 5, 255, 172, 30), 32, 8, ILI9341_WHITE);
    brightness_old = brightness;
    OCR0A = brightness;
  }
}

void screenbrightnessAuto() {
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
  OCR0A = brightness;
}
