// GameColour.h

#ifndef _GAMECOLOUR_h
#define _GAMECOLOUR_h

#include <stdint.h> // for whatever reason uint8_t didn't get recognised

// Typedefs
typedef uint16_t colour;

// Struct
struct rgb {
	uint8_t r; 
	uint8_t g; 
	uint8_t b; 
};

class GameColour
{
public:
	GameColour();
	void generateNewColour(); // Randomly generates a new colour
	void setGameColour(uint8_t red, uint8_t green, uint8_t blue);
	colour color565(uint8_t red, uint8_t green, uint8_t blue);
	
	// getters
	colour getGameColour();
	colour getGameColour_025();
	colour getGameColour_050();
	colour getGameColour_negative();
	colour getGameColour_negative_050();
	
	colour getBackgroundColour();
	colour getBackgroundColour_light();
private:
	rgb getRGBfromHue(uint16_t hue);

	colour gameColour;
	colour gameColour_025;
	colour gameColour_050;
	colour gameColour_negative;
	colour gameColour_negative_050; 
	
	colour backgroundColour;
	colour backgroundColour_light;
};


#endif

