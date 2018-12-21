#include "gameobject.h"

Gameobject::Gameobject(int id) {
    this->id = id;
}

Gameobject::~Gameobject() {
    printf("%s: %i\n", "verwijder", id);
}