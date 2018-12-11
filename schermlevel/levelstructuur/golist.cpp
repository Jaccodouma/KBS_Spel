#include "golist.h"

void GoList::add(Gameobject *go) {
    node *n = new node;
    n->data = go;
    n->next = NULL;

    if (head == NULL) {  // lege lijst
        head = n;
        tail = n;
    } else {  // plaats achteraan de lijst
        tail->next = n;
        tail = n;
    }
}

int GoList::length() {
    int length = 0;
    if (head != NULL) {  // geen lege lijst
        node *temp = head;
        do {
            length++;
            temp = temp->next;
        } while (temp != NULL);
    }
    return length;
}
#include <stdio.h>
void GoList::del(Gameobject *go) {
    node *tempprev = NULL;
    node *temp = head;
    while (temp != NULL) {  // geen lege lijst
        if (temp->data == go) { // gevonden
            if (tempprev == NULL) { // go zat in de head-node
                head = temp->next;
            } else {
                tempprev->next = temp->next;
            }
            if (temp->next == NULL) {
                tail = tempprev;
            }
            // Geef het gealloceerde geheugen vrij
            delete temp->data;
            delete temp; // verwijder node
            temp = NULL;
        } else {
            tempprev = temp;
            temp = temp->next;
        }
    }
}