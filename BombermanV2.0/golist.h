#pragma once

#include <string.h>
#include "gameobject.h"

struct node {
    Gameobject *data = NULL;
    node *next = NULL;
};

class GoList {
   public:
    ~GoList();
    void add(Gameobject *go);
    void del(Gameobject *go);
    int length();
    Gameobject *getNext();
    node *head = NULL, *tail = NULL;

   private:
    node *next = NULL;
};
