#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "golist.h"
#include <cassert>

using namespace std;

int main(int argc, char const *argv[]) {
    GoList list;

    for (int i=0; i<10; i++) {
        list.add(new Gameobject(i));
    }
    assert(list.length() == 10);

    Gameobject *temp = list.getNext();
    while (temp != NULL) {
        if (temp->id > 2 && temp->id < 5) {
            list.del(temp);
        } else {
            printf("%i,", temp->id);
        }
        temp = list.getNext();
    }
    printf("\n");

    assert(list.length() == 8);

    printf("einde programma\n");

    return 0;
}