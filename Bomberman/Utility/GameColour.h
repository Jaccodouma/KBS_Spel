// GameColour.h

#ifndef _GAMECOLOUR_h
#define _GAMECOLOUR_h

#include <stdint.h> // for whatever reason uint8_t didn't get recognised

// Typedefs
typedef uint16_t colour;

class GameColour
{
public:
	GameColour();
	void generateNewColour(); // Randomly generates a new colour
	void setGameColour(uint8_t red, uint8_t green, uint8_t blue);
	colour color565(uint8_t red, uint8_t green, uint8_t blue);
	
	// getters
	colour getGameColour();
	colour getGameColour_050();
	colour getBackgroundColour();
	colour getGameColour_negative();
private:
	colour gameColour;
	colour gameColour_050;
	colour backgroundColour;
	colour gameColour_negative;
};


#endif

