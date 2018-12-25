#include "game.h"
#include "explosion.h"

Game::Game(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;
}

Game::~Game() {
    if (players[0] == NULL) delete players[0];
    if (players[1] == NULL) delete players[1];
}

void Game::addRandomBlocks() {
    randomSeed(analogRead(0));      // voor de random-functie
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
                        gos.add(new Block(j, i));
                    }
                }
            }
        }
    }
}

void Game::update(Gfx *gfx) {
    Gameobject *go = gos.getNext();
    while (go != NULL) {
        if (needsUpdate(go)) {
            go->update(millis() - prevUpdate);
        }
        if (needsRedraw(go)) {
            go->draw(gfx);
        }
        if (needsDelete(go)) {
            go->onDelete(gfx);
            gos.del(go);
        }
        go = gos.getNext();
    }
    prevUpdate = millis();
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
    addRandomBlocks();
    if (players[0] != NULL) {  // Start bij tenminste één speler
        this->started = true;
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
        Gameobject *collision = hasCollision(p, nextpos);
        if (collision == NULL) {
            p->move(d);
            return;
        }
        collision->onPlayerCollision(p);
        if (!isSolid(collision)) {
            p->move(d);
            toggleRedraw(collision);
        }
    }
}

void Game::addGameobject(Gameobject *go) { gos.add(go); }

void Game::bombExplosion(Bomb *bomb) {
    char x = bomb->getFieldPos().x;
    char y = bomb->getFieldPos().y;
    gos.add(new Explosion(x, y, bomb->player));       // vuur op plaast van bom
    for (char i = x; i > x - EXPLOSION_RANGE; i--) {  // vuur links
        if (!addExplosion(i - 1, y, bomb->player)) break;
    }
    for (char i = x; i < x + EXPLOSION_RANGE; i++) {  // vuur rechts
        if (!addExplosion(i + 1, y, bomb->player)) break;
    }
    for (char i = y; i > y - EXPLOSION_RANGE; i--) {  // vuur boven
        if (!addExplosion(x, i - 1, bomb->player)) break;
    }
    for (char i = y; i < y + EXPLOSION_RANGE; i++) {  // vuur onder
        if (!addExplosion(x, i + 1, bomb->player)) break;
    }
}

bool Game::addExplosion(char x, char y, Player *p) {
    position pos = {x, y};
    if (gridCollision(pos)) {
        return false;
    }
    Gameobject *co = hasCollision(p, pos);  // tegenaan botsend object
    if (co) {
        co->onExplosion(p);
        if (isSolid(co)) {
            gos.add(new Explosion(x, y, p));
            return false;
        }
    }
    gos.add(new Explosion(x, y, p));
    return true;
}

bool Game::isStarted() { return this->started; }

uint8_t Game::getWidth() { return this->width; }

uint8_t Game::getHeight() { return this->height; }