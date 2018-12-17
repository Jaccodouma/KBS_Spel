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
	colour getGameColour(); // Returns colour
	void setGameColour(colour gameColour);
	void setGameColour(uint8_t red, uint8_t green, uint8_t blue);
	colour color565(uint8_t red, uint8_t green, uint8_t blue);
private:
	colour gameColour;
};


#endif

