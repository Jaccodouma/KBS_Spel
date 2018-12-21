#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char naam[9];
} Student;

void verander(Student s) {
    strcpy(s.naam, "HALLOO");
}

int main(int argc, char const *argv[])
{
    Student s;
    s.id = 1094012;
    strcpy(s.naam, "Floris");
    verander(s);
    printf("%s, (%i)\n", s.naam, s.id);

    return 0;
}
