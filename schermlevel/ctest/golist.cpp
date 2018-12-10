#include "golist.h"

void List::add(Gameobject *go) {
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

int List::size() {
    int size = 0;
    if (head != NULL) {  // geen lege lijst
        node *temp = head;
        do {
            size++;
            temp = temp->next;
        } while (temp != NULL);
    }
    return size;
}