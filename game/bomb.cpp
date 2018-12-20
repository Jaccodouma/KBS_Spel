#include "bomb.h"

Bomb::Bomb(int x, int y, Player *p) : Gameobject(x, y, true) {
    toggleRedraw(this);
    toggleUpdate(this);
} 


void Bomb::draw(Gfx *gfx) {
    if(state == 0) {
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[0], 16, 16, DARKGREY);
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[1], 16, 16, YELLOW);
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[2], 16, 16, DARKORANGE);
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[3],16, 16,  LIGHTGREY);
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[4], 16, 16, WHITE);
    
    } if(state == 1) {
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[1], 16, 16, RED);
    gfx ->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitBomb[2], 16, 16, YELLOW);
    } 
    if (state == 2){
        gfx->tft.fillRect(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, gfx->blocksize, gfx->blocksize, CLR_BACKGROUND);
        deleteObject(this);
        gfx->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitExplosion[0], 16, 16, RED);
        gfx->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitExplosion[1], 16, 16, DARKORANGE);
        gfx->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitExplosion[2], 16, 16, LIGHTORANGE);
        gfx->tft.drawBitmap(fieldPos.x * gfx->blocksize + gfx->offsetX, fieldPos.y * gfx->blocksize + gfx->offsetY, bitExplosion[3], 16, 16,YELLOW);
    }
    



    //toggleRedraw(this);

}

void Bomb::update(){
    long current_time = millis();
    state = !state;
    // Serial.println(current_time - prev_millis);

    countdown -= (current_time - prev_millis);
    Serial.println(countdown);
   
 
   
    if(countdown <= 0){
    state = 2;
}

    if(state == 0){
    if((countdown / 2) <= 0){
    
    }
    }
    prev_millis = current_time;

}