#include <stdio.h>
#include "level.h"
#include "gameobject.h"

int main(void)
{
    init();
    Serial.begin(9600);
    Serial.println("Welkom\n");
    Level *l = new Level(15,15);
    l->printLevel();
    Serial.println(sizeof(l));
    while(1) {

    }
}