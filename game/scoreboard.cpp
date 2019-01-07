#include "scoreboard.h"
#include "game.h"
#include "player.h"

Scoreboard::Scoreboard(Gfx *gfx, Adafruit_ILI9341 *tft) {
  this->gfx = gfx; 
  this->tft = tft; 
  }

void Scoreboard::init(Player *players[]) {
    uint16_t cursorX = 0, cursorY = 0;

    for (int i = 0; i < MAXNPLAYER; i++) {
        // buffer iets vergroot omdat geheugenlek?
        char buffer[10 + (MAXNAMELENGTH + 1) + 1];  // name + '\0' + :
        // lukt op een of andere manier niet om de waarde 6 als variabele mee te
        // geven
        sprintf(buffer, "%-6s:", players[i]->name);  // reserveer 6 plaatsen.
        gfx->drawText(cursorX, cursorY, buffer);

        cursorX = MAXNAMELENGTH * (TEXTWIDTH - 1);
        gfx->drawXBitmap(0 - gfx->offsetX + cursorX, 0 - gfx->offsetY + cursorY,
                         hearth[0], DARKBROWN);
        gfx->drawXBitmap(0 - gfx->offsetX + cursorX, 0 - gfx->offsetY + cursorY,
                         hearth[1], RED);
        gfx->drawXBitmap(0 - gfx->offsetX + cursorX, 0 - gfx->offsetY + cursorY,
                         hearth[2], WHITE);

        gfx->drawChar(cursorX + 22, cursorY, 'X');

        cursorX = 0;
        cursorY += TEXTWIDTH + 1;
    }

    update(players);
}

void Scoreboard::update(Player *players[]) {
    uint16_t cursorX, cursorY = 0;

    for (int i = 0; i < MAXNPLAYER; i++) {
        playerinfo pinfo = players[i]->getPlayerinfo();

        cursorX = 130;
        char buffer[12];
        sprintf(buffer, "%i, %ip", pinfo.lives, pinfo.score);

        tft->fillRect(130, cursorY, 120, 16, CLR_BACKGROUND);
        gfx->drawText(cursorX, cursorY, buffer);
        cursorY += TEXTWIDTH + 1;
    }
}
