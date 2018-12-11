#pragma once

#include "gameobject.h"
#include <string.h>


struct node {
    Gameobject *data;
    node *next;
};

class GoList {
    public:
    void add(Gameobject *go);
    int length();
    void del(Gameobject *go);

    private:
    node *head=NULL, *tail=NULL;
};