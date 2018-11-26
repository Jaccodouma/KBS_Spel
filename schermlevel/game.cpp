// Auteur: Floris Folkertsma (s1094012)
#include "game.h"

Game::Game(Level *level, Player *player, Gfx *gfx) {
    this->level = level;
    this->player = player;
    this->gfx = gfx;
}

void Game::start() {
    this->gfx->drawLevel(this->level);
    this->gfx->drawPlayer(this->player);
}

void Game::movePlayer(direction d) {
    
}