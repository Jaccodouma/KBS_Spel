#include <iostream>

using namespace std;

class Hoi {
   public:
    string hoihoi;
    Hoi() { this->hoihoi = "standaard"; }
};

void verander(Hoi *hoi) { hoi->hoihoi = "123"; }

int main(int argc, char const *argv[]) {
    Hoi *h1 = new Hoi();
    verander(h1);
    cout << h1->hoihoi << endl;
    return 0;
}