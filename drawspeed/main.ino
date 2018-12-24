// expiriment 1: kijk hoe de bitmap van een poppetje met 5 kleuren kost ~10ms
// bitmap uit flashgeheugen halen: <1ms
// zwarte bitmap over poppetje tekenen: ~6ms
// zwart vierkant over poppetje: <1ms
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define CLR_BACKGROUND 0x0000
#define CLR_BACK_HI Screen.color565(150, 150, 150)
#define CLR_TEXT Screen.color565(0, 0, 0)
#define CLR_INDICATOR Screen.color565(255, 0, 255)

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define PURPLE 0x780F
#define WHITE 0xFFFF
#define SKIN 0xFEB8

#define BLOCKSIZE 16

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

const uint8_t poppetjevoledig[] PROGMEM = {
    0x1F, 0xF8, 0x1F, 0xF8, 0x18, 0x18, 0x10, 0x08, 0x14, 0x48, 0x10,
    0x08, 0x18, 0x18, 0x0F, 0xF0, 0x02, 0x40, 0x3E, 0x7C, 0x7F, 0xFE,
    0x7F, 0xFE, 0x7F, 0xFE, 0x1F, 0xF8, 0x13, 0xC8, 0xBE, 0x4D};

const uint8_t player_still[5][32] PROGMEM = {
    {0b00011111, 0b11111000, 0b00010000, 0b00001000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00011111, 0b11111000, 0b00111111, 0b11111100,
     0b00001111, 0b11110000, 0b00001111, 0b11110000, 0b10100000, 0b00000101,
     0b00010010, 0b01001000},
    {0b00000000, 0b00000000, 0b00001111, 0b11110000, 0b00011000, 0b00011000,
     0b00010000, 0b00001000, 0b00010100, 0b01001000, 0b00010000, 0b00001000,
     0b00011000, 0b00011000, 0b00001111, 0b11110000, 0b00000010, 0b01000000,
     0b00111110, 0b01111100, 0b01100000, 0b00000110, 0b01000000, 0b00000010,
     0b01110000, 0b00001110, 0b00010000, 0b00001000, 0b00010011, 0b11001000,
     0b00010010, 0b01001000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000111, 0b11100000,
     0b00001001, 0b10010000, 0b00001001, 0b10010000, 0b00001111, 0b11110000,
     0b00000111, 0b11100000, 0b00000000, 0b00000000, 0b00000001, 0b10000000,
     0b00000001, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b01000000, 0b00000010, 0b01000000, 0b00000010,
     0b00000000, 0b00000000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001100, 0b00110000,
     0b00001100, 0b00110000},
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000110, 0b01100000, 0b00000010, 0b00100000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
     0b00000000, 0b00000000}};

const uint8_t dvdvideo[] PROGMEM = {
    0x1F, 0xFE, 0x0F, 0xF8, 0x3F, 0xFE, 0x1F, 0xFE, 0x38, 0x7E, 0x3B,
    0x8F, 0x38, 0x77, 0x7B, 0x87, 0x38, 0x77, 0x77, 0x07, 0x38, 0xE7,
    0xE7, 0x0E, 0x7F, 0xC3, 0xC7, 0xFC, 0x7F, 0x03, 0x87, 0xF0, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xC0,
    0x7F, 0xF8, 0x7F, 0xFC, 0xFF, 0xF0, 0x3F, 0xFC, 0x07, 0xFF, 0xFF,
    0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x4E,
    0x33, 0x80, 0x0C, 0x4A, 0x32, 0x40, 0x04, 0x4E, 0x33, 0x80,
};

int main(void) {
    init();
    Serial.begin(9600);

    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    tft.begin();
    tft.fillScreen(CLR_BACKGROUND);

    unsigned long tijd;

    int x = 1, y = 1, i = 2, j = 2;

    uint16_t colors[] = {WHITE, BLUE, RED, GREEN, YELLOW};
    uint8_t coln = 0;

    while (1) {
        tijd = millis();

        //_delay_ms(10);
        tft.drawBitmap(x, y, dvdvideo, 32, 19, CLR_BACKGROUND);

        // tft.drawBitmap(0, 0, poppetjevoledig,  BLOCKSIZE,  BLOCKSIZE, BLACK);
        // tft.drawBitmap(0, 0, player_still[1],  BLOCKSIZE,  BLOCKSIZE, BLACK);
        // tft.drawBitmap(0, 0, player_still[2],  BLOCKSIZE,  BLOCKSIZE, BLACK);
        // tft.drawBitmap(0, 0, player_still[3],  BLOCKSIZE,  BLOCKSIZE, BLACK);
        // tft.drawBitmap(0, 0, player_still[4],  BLOCKSIZE,  BLOCKSIZE, BLACK);

        x += i;
        y += j;
        if (x >= tft.width() - 32 || x <= 0) {
            i = -i;
            if (coln + 1 > 4) {
                coln = 0;
            } else {
                coln++;
            }
        }
        if (y >= tft.height() - 19 || y <= 0) {
            j = -j;
            if (coln + 1 > 4) {
                coln = 0;
            } else {
                coln++;
            }
        }
        tft.drawBitmap(x, y, dvdvideo, 32, 19, colors[coln]);
        // tft.drawBitmap(x, y, player_still[0], BLOCKSIZE, BLOCKSIZE, RED);
        // tft.drawBitmap(x, y, player_still[1], BLOCKSIZE, BLOCKSIZE, BLACK);
        // tft.drawBitmap(x, y, player_still[2], BLOCKSIZE, BLOCKSIZE, SKIN);
        // tft.drawBitmap(x, y, player_still[3], BLOCKSIZE, BLOCKSIZE, YELLOW);
        // tft.drawBitmap(x, y, player_still[4], BLOCKSIZE, BLOCKSIZE, WHITE);

        Serial.println(millis() - tijd);
    }
    return 0;
}