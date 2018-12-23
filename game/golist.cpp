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
    freeRam();
}

GoList::~GoList() {
    // destructor, geef al het gealloceerde gehuegen vrij
    node *tempprev = NULL;
    node *temp = head;
    while (temp != NULL) {
        tempprev = temp;
        temp = tempprev->next;
        delete tempprev->data;
        delete tempprev;  // verwijder node
    }
}

void GoList::del(Gameobject *go) {
    node *tempprev = NULL;
    node *temp = head;
    while (temp != NULL) {           // geen lege lijst
        if (temp->data == go) {      // gevonden
            if (tempprev == NULL) {  // go zat in de head-node
                head = temp->next;
            } else {
                tempprev->next = temp->next;
            }
            if (temp->next == NULL) {
                tail = tempprev;
            }
            // Geef het gealloceerde geheugen vrij
            delete temp->data;
            delete temp;  // verwijder node
            temp = NULL;
        } else {
            tempprev = temp;
            temp = temp->next;
        }
    }
        freeRam();

}

int GoList::length() {
    // berekent de lengte van de lijst
    int l = 0;
    node *temp = head;
    while (temp != NULL) {
        l++;
        temp = temp->next;
    }
    return l; 
}

Gameobject *GoList::getNext() {
    if (head == NULL) {
        return NULL;  // lege lijst
    }
    if (next == NULL) {
        next = head;
        return next->data;
    }
    next = next->next;
    if (next != NULL) {
        return next->data;
    }
    return NULL;
}