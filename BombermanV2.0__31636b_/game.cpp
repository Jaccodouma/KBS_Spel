#include "game.h"

Game::Game(uint8_t width, uint8_t height, Adafruit_ILI9341 *tft, Gfx *gfx, Scoreboard *scoreboard, IR *ir) {
  this->width = width;
  this->height = height;
  this->tft = tft;
  this->gfx = gfx;
  this->ir = ir;
  this->scoreboard = scoreboard;
}

Game::~Game() {
  for (int i = 0; i < MAXNPLAYER; i++) {
    if (players[i] != NULL) delete players[i];
  }
}

void Game::addRandomBlocks() {
    randomSeed(240);  // voor de random-functie
    uint8_t n = 0;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            // ga langs alle posities maar sla grid-blokjes over
            if (!isEven(i) || !isEven(j)) {
                // kans is 1 op 3 dat er een blokje geplaatst wordt
                if (random() % 3 == 0) {
                    if (!(i == 1 && j == 1) &&
                        // mag niet helemaal linksonder of rechtsonder omdat
                        // players hier komen
                        !(i == height - 2 && j == width - 2)) {
                        if (n < MAXNBLOCKS) {
                            blocks[n] = width * i + j;
                            n++;
                        }
                    }
                }
            }
        }
    }
    for (; n < MAXNBLOCKS; n++) {  // clear de rest
        blocks[n] = 0;
    }
}

void Game::update() {
  int div = ir->getTime_ms() - prevUpdate;  // verschil in ms sinds laatste verversing
  if (div >= REFRESHRATE) {
    prevUpdate = ir->getTime_ms();  // tijdstip van updaten
    node *prevNode = NULL;  // pointer om vorige node bij te houden
    node *n = gos.head;     // huidige node, begin bij het hoofd van de
    // gelinkte lijst
    while (n != NULL) {
      Gameobject *go = n->data;
      if (needsUpdate(go)) {
        go->update(div);
      }
      if (needsDelete(go)) {
        go->onDelete(gfx);  // voer onDelete uit op desbetreffende object
        if (prevNode ==
            NULL) {  // betekent dus dat head verwijderd wordt.
          gos.head = n->next;  // link de volgende van de head node
          // aan de head-pointer
        } else {
          prevNode->next = n->next;  // haal de schakel er tussen uit
        }
        if (n->next == NULL) {  // als de volgende van de huidige node
          // geen volgende had
          gos.tail =
            prevNode;  // betekent dit dus dat het de laatste node
          // was, dus huidige is tevens de staart
        }

        // // Geef het gealloceerde geheugen vrij
        delete go;  // verwijder gameobject
        node *temp =
          n->next;  // referentie naar volgende node tijdelijk bewaren
        delete n;     // verwijder huidige node

        n = temp;  // huidige wordt volgende

        continue;  // sla wat hier onder verder gebeurt over en itereer
        // verder
      }
      if (needsRedraw(go)) {
        go->draw(gfx);
      }
      prevNode = n;
      n = n->next;
    }
  }
}

bool Game::gridCollision(position p) {
  if (p.y <= 0 || p.y >= height - 1) {
    // botsing met bovenste of onderste blokjes of buiten het speelveld
    return true;
  }
  if (p.x <= 0 || p.x >= width - 1) {
    // botsing met blokjes aan de zijkanten van veld of buiten het speelveld
    return true;
  }
  if (isEven(p.x) && isEven(p.y)) {
    // botsing met rasterblokjes
    return true;
  }
  return false;
}

int Game::blockCollision(position p) {
    for (int i = 0; i < MAXNBLOCKS; i++) {
        uint8_t block = blocks[i];
        if (block == 0) {
            continue;
        }
        uint8_t x = block % width;
        uint8_t y = block / width;
        if (y == p.y && x == p.x) {
            return i;
        }
    }
    return -1;
}

Gameobject *Game::hasCollision(Gameobject *go, position p) {
  Gameobject *temp = gos.getNext();
  Gameobject *collision = NULL;
  while (temp != NULL) {
    position pos = temp->getFieldPos();
    if (p.x == pos.x && p.y == pos.y) {  // botsing met een game-object
      collision = temp;
    }
    temp = gos.getNext();
  }
  return collision;
}

bool Game::start() {
  if (players[0] != NULL) {  // Start bij tenminste één speler
    this->started = true;
    gfx->tft->fillScreen(CLR_BACKGROUND);
    drawLevel();
    addRandomBlocks();
    scoreboard->init(players);
  }
  return this->started;
}

bool Game::addPlayer(Player *p) {
  if (players[0] == NULL) {
    players[0] = p;
    gos.add(p);  // voeg toe aan de gameobject-lijst
    return true;
  }
  if (players[1] == NULL) {
    players[1] = p;
    gos.add(p);  // voeg toe aan de gameobject-lijst
    return true;
  }
  return false;  // genoeg spelers
}

void Game::movePlayer(Player *p, direction d) {
    // laat de alleen bewegen als de speler stilstaat en de volgende positie
    // geen bostsing zou veroorzaken
    if (!p->isMoving()) {
        position nextpos = movePosition(p->getFieldPos(), d);

        if (gridCollision(nextpos)) {
            return;  // heeft een botsing tegen de vast blokjes
        }
        if (blockCollision(nextpos) != -1) {
            return;
        }
        // zoek een gameobject waar de speler tegenaan botst.
        Gameobject *collision = hasCollision(p, nextpos);
        if (collision == NULL) {  // de speler botst tegen geen enkel object
            p->move(d);           // loop 1 blokje in die richting
            return;
        }
        // speler botst wel tegen een object
        collision->onPlayerCollision(
            p);  // roep deze functie aan op het desbetreffende object
        if (!isSolid(collision)) {  // als het object doordringbaar is, loop er
            // doorheen en herteken het
            p->move(d);  // loop 1 blokje in die richting
            redraw(collision);
        }
    }
}

void Game::addGameobject(Gameobject *go) {
  gos.add(go);
}

void Game::bombExplosion(Bomb *bomb) {
  char x = bomb->getFieldPos().x;
  char y = bomb->getFieldPos().y;
  gos.add(new Explosion(x, y, bomb->player));       // vuur op plaats van bom
  for (char i = x; i > x - EXPLOSION_RANGE; i--) {  // vuur links
    if (!addExplosion(i - 1, y, bomb->player, DIR_LEFT,
                      (i == x - EXPLOSION_RANGE + 1)))
      break;
  }
  for (char i = x; i < x + EXPLOSION_RANGE; i++) {  // vuur rechts
    if (!addExplosion(i + 1, y, bomb->player, DIR_RIGHT,
                      (i == x + EXPLOSION_RANGE - 1)))
      break;
  }
  for (char i = y; i > y - EXPLOSION_RANGE; i--) {  // vuur boven
    if (!addExplosion(x, i - 1, bomb->player, DIR_UP,
                      (i == y - EXPLOSION_RANGE + 1)))
      break;
  }
  for (char i = y; i < y + EXPLOSION_RANGE; i++) {  // vuur onder
    if (!addExplosion(x, i + 1, bomb->player, DIR_DOWN,
                      (i == y + EXPLOSION_RANGE - 1)))
      break;
  }
}

bool Game::addExplosion(char x, char y, Player *p, direction dir, bool last) {
    // deze functie geeft false terug als de explosie moet stoppen
    position pos = {x, y};
    if (gridCollision(pos)) {  // heeft een botsing tegen een grijs blokje
        return false;
    }
    int blockindex = blockCollision(pos);
    if (blockindex != -1) {
        blocks[blockindex] = 0;  // blokje weg

        p->giveScore(POINTSBLOCKDESTROY);
        addCornerExplosion(x, y, p, dir);
        return false;
    }
    Gameobject *co = hasCollision(p, pos);  // tegenaan botsend object
    if (co) {
        co->onExplosion(
            p);  // roep onExplosion op het desbetreffende geraakte object aan
        if (isSolid(co)) {  // als het een ondoordringbaar object is zal het
            // gesloopt worden
            addCornerExplosion(x, y, p, dir);

            return false;  // voorkomt dat de explosie verder dan het gesloopte
                           // blokje gaat
        }
    }
    if (!last) {
        if (dir == DIR_LEFT || dir == DIR_RIGHT) {
            gos.add(new Explosion(x, y, p, EX_HORIZONTAL));
        } else if (dir == DIR_UP || dir == DIR_DOWN) {
            gos.add(new Explosion(x, y, p, EX_VERTICAL));
        }
    } else {
        addCornerExplosion(x, y, p, dir);
    }

    return true;  // niks aan de hand, geen botsingen
}

void Game::addCornerExplosion(char x, char y, Player *p, direction dir) {
  switch (dir) {
    case direction::DIR_LEFT:
      gos.add(new Explosion(x, y, p, EX_LEFT));
      break;
    case direction::DIR_RIGHT:
      gos.add(new Explosion(x, y, p, EX_RIGHT));
      break;
    case direction::DIR_UP:
      gos.add(new Explosion(x, y, p, EX_TOP));
      break;
    case direction::DIR_DOWN:
      gos.add(new Explosion(x, y, p, EX_BOT));
      break;
  }
}

void Game::drawLevel() {
  gfx->blocksize = gfx->tft->width() / width;
  // stel de offsetX in zodat het speelveld horizontaal gecentreerd is.
  gfx->offsetX = (gfx->tft->width() - width * gfx->blocksize) / 2;
  // stel de offsetY in zodat het speelveld onderin het scherm eindigt.
  gfx->offsetY = (gfx->tft->height() - height * gfx->blocksize);
  gfx->tft->fillRect(gfx->offsetX, gfx->offsetY,
                   gfx->tft->width() - gfx->offsetX * 2,
                   gfx->tft->height() - gfx->offsetY * 2, CLR_BACKGROUND);
  gfx->tft->drawRect(gfx->offsetX, gfx->offsetY,
                   gfx->tft->width() - gfx->offsetX * 2,
                   gfx->tft->height() - gfx->offsetY * 2, BLACK);
  drawGrid();
  drawBlocks();
}
void Game::drawGrid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1) {  // bovenste of onderste rij
                drawBlock(j, i, LIGHTGREY, DARKGREY);
            } else {
                if (j == 0 || j == width - 1) {  // rand links of rechts
                    drawBlock(j, i, LIGHTGREY, DARKGREY);
                } else {
                    if (isEven(i) && isEven(j)) {
                        drawBlock(j, i, LIGHTGREY, DARKGREY);
                    }
                }
            }
        }
    }
}

void Game::drawBlocks() {
    for (int i = 0; i < MAXNBLOCKS; i++) {
        if (blocks[i] == 0) {
            continue;
        }
        drawBlock(blocks[i] % width, blocks[i] / width, PURPLE, DARKGREY);
    }
}

void Game::drawBlock(uint8_t x, uint8_t y, uint16_t colorfill,
                     uint16_t colordraw) {
    gfx->drawRectField(x, y, colorfill);
    gfx->drawRectField(x, y, colordraw, false);
}

void Game::updateScores(Player *p) {
  scoreboard->update(players);
}

bool Game::isStarted() {
  return this->started;
}

uint8_t Game::getWidth() {
  return this->width;
}

uint8_t Game::getHeight() {
  return this->height;
}
