#pragma once

#include "gameobject.h"
#include <string.h>


struct node {
    Gameobject *data;
    node *next;
};

class GoList {
    public:
    ~GoList();
    void add(Gameobject *go);
    void del(Gameobject *go);
    int length();
    Gameobject * getNext();

    private:
    node *head=NULL, *tail=NULL, *next=NULL;
};