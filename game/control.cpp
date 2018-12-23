#include "control.h"

Control::Control(View *view) {
    this->view = view;
    this->nunchuk = ArduinoNunchuk();
    // Initialize the nunchuk
    this->nunchuk.init();
}

void Control::startGame() {
    game = new Game(15, 15);
    Player *p = new Player("ffk27", 1, 1);
    game->addPlayer(p);
    Player *p2 = new Player("Merel", 13, 13);
    game->addPlayer(p2);
    game->start();
    view->drawLevel(game);  // teken het level-grid
    Serial.println("Game started");
}

void Control::update() {
    this->nunchuk.update();
    direction dir = nunchuck_Direction();
    if (game->isStarted()) {
        game->update(&view->gfx);
        movePlayer(static_cast<direction>(random(5)));
        game->movePlayer(game->players[1], static_cast<direction>(random(5)));
        // movePlayer(game->players[0], dir);
        // movePlayer(game->players[1], dir);
    }
}

void Control::movePlayer(direction d) {
    game->movePlayer(game->players[0], d);
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