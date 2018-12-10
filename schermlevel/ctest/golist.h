#pragma once

#include "gameobject.h"
#include <string.h>


struct node {
    Gameobject *data;
    node *next;
};

class List {
    public:
    void add(Gameobject *go);
    int size();

    private:
    node *head=NULL, *tail=NULL;
};