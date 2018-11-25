#include <stdio.h>
#include <stdlib.h>

void print2darray(int m, int n, char **chars) {
    for (int i=0; i<m; i++) {
        printf("%c", '\n');
        for (int j=0; j<n; j++) {
            printf("%c", chars[i][j]);
        }
    }
}

int main(int argc, char const *argv[])
{
    int width=2,height=2;
    char **chars = (char **)malloc(height * sizeof(char *));
    for (int i=0; i<height; i++) {
        chars[i] = (char *)calloc(width, sizeof(char));
    }
    chars[0][0] = 'A';
    chars[0][1] = 'B';
    chars[1][0] = 'C';
    chars[1][1] = 'D';
    print2darray(2, 2, chars);
    return 0;
}
