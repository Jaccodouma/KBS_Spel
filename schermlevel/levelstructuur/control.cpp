#include "control.h"

Control::Control(View *view) {
    this->view = view;
    this->nunchuk = ArduinoNunchuk();
    // Initialize the nunchuk
    this->nunchuk.init();
}

void Control::startGame() {
    game = new Game(15, 17);
    Player *p = new Player("ffk27", 1, 1, view->blockSize());
    game->addPlayer(p);
    Player *p2 = new Player("Spiel", 13, 15, view->blockSize());
    game->addPlayer(p2);
    game->start();
    view->drawLevel(game);  // teken het level-grid
    Serial.println("Game started");
}

void Control::update() {
    this->nunchuk.update();
    direction dir = nunchuck_Direction();
    if (game->isStarted()) {
        movePlayer(game->players[0], static_cast<direction>(random(5)));
        //movePlayer(game->players[1], static_cast<direction>(random(5)));
        //movePlayer(game->players[0], dir);
        movePlayer(game->players[1], dir);
    }
    game->update(view->gfx);
}

void Control::movePlayer(Player *p, direction d) {
    // laat de alleen bewegen als de speler stilstaat en de volgende positie
    // geen bostsing zou veroorzaken
    if (!p->isMoving()) {
        position nextpos = movePosition(p->getFieldPos(), d);
        if (game->gridCollision(nextpos)) {
            Serial.println("grid collision");
            return; // heeft een botsing tegen de vast blokjes
        }
        Gameobject * collision = game->hasCollision(p, nextpos);
        if (collision == NULL) {
            p->move(d);
            return;
        }
        printPosition(collision->getFieldPos());
                    Serial.println("gameobject");
        if (!collision->solid) {
            p->move(d);
            toggleRedraw(collision);
            toggleRedraw(p);
        }
    }
}

direction Control::nunchuck_Direction() {
    if (nunchuk.analogX >= 0 && nunchuk.analogX <= 64) {  // to the left
        return direction::DIR_LEFT;
    }

    if (nunchuk.analogX >= 192 && nunchuk.analogX <= 255) {  // to the right
        return direction::DIR_RIGHT;
    }

    if (nunchuk.analogY >= 0 && nunchuk.analogY <= 64) {  // down
        return direction::DIR_DOWN;
    }

    if (nunchuk.analogY >= 192 && nunchuk.analogY <= 255) {  // up
        return direction::DIR_UP;
    }
    return direction::DIR_NO;
}