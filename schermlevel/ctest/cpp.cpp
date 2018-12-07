#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct Student {
    int id;
    char naam[9];
};

class Hoi {
   public:
    string hoihoi;
    Hoi() { this->hoihoi = "standaard"; }
};

// void verander(Hoi *hoi) { hoi->hoihoi = "123"; }

int verander(int a[]) {
    a[0] = 0;
}

int main(int argc, char const *argv[]) {
    // Hoi *h1 = new Hoi();
    // verander(h1);
    // cout << h1->hoihoi << endl;
    struct Student s;
    s.id = 1094012;
    int arrayp[] = {1,2,3,4,5,6,7,8,9};
    //verander(arrayp);
    strcpy(s.naam, "Floris");
    cout << sizeof(Hoi) << endl;
    Hoi *hoi = (Hoi*)malloc(sizeof(Hoi));
    return 0;
}