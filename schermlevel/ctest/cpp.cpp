#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "golist.h"

using namespace std;

int main(int argc, char const *argv[]) {
    List list;
    printf("%i\n", list.size());
    list.add(new Gameobject);
    printf("%i\n", list.size());
    list.add(new Gameobject);

    list.add(new Gameobject);

    list.add(new Gameobject);

    list.add(new Gameobject);

    list.add(new Gameobject);
    printf("%li\n", sizeof(list));

    return 0;
}