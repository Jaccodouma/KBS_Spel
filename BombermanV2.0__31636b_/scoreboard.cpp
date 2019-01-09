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
        gfx->drawText(cursorX, cursorY, "P");

        cursorX = 6 * (TEXTWIDTH - 1);
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

        cursorX = 130;
        char buffer[12];
        sprintf(buffer, "%i, %ip", players[i]->lives, players[i]->score);

        tft->fillRect(130, cursorY, 120, 16, CLR_BACKGROUND);
        gfx->drawText(cursorX, cursorY, buffer);
        cursorY += TEXTWIDTH + 1;
    }
}
