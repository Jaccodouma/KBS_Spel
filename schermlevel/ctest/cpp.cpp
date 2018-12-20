#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "golist.h"

using namespace std;

int main(int argc, char const *argv[]) {
    GoList list;

    Gameobject *ggoo = new Gameobject(1);
    Gameobject *ggoo2 = new Gameobject(2);
    Gameobject *ggoo3 = new Gameobject(3);


    list.add(ggoo);
        printf("%i\n", list.length());



    list.del(ggoo);

        printf("%i\n", list.length());

    list.add(ggoo2);
    printf("%i\n", list.length());

    // list.add(ggoo3);
    //     printf("%i\n", list.length());

    // list.del(ggoo2);
    //     list.del(ggoo3);

    return 0;
}