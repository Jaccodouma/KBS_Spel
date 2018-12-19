#include "bomb.h"




Bomb::Bomb(int x, int y, Player *p) : Gameobject(x, y, true) {
    toggleRedraw(this);
} 


void Bomb::draw(Gfx *gfx) {
// gfx->tft.fillCircle(fieldPos.x * gfx->blocksize  + gfx->offsetX + gfx->blocksize / 2 , fieldPos.y  * gfx->blocksize  + gfx->offsetY + gfx->blocksize / 2, gfx->blocksize / 2, CYAN);

    gfx ->tft.drawBitmap(fieldPos.x, fieldPos.y, bitBomb[0], 16, 16, DARKGREY);
    gfx ->tft.drawBitmap(fieldPos.x, fieldPos.y, bitBomb[1], 16, 16, YELLOW);
    gfx ->tft.drawBitmap(fieldPos.x, fieldPos.y, bitBomb[2], 16, 16, DARKORANGE);
    gfx ->tft.drawBitmap(fieldPos.x, fieldPos.y, bitBomb[3],16, 16,  LIGHTGREY);
    gfx ->tft.drawBitmap(fieldPos.x, fieldPos.y, bitBomb[4], 16, 16, WHITE);

    toggleRedraw(this);
}

void Bomb::update(){

}