#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    freopen(argv[1], "w", stdout);
    char c = (argv[2][0] == '0' ? 0 : 1);
    int k = atoi(argv[3]);
    for (int i = 0; i < k; i++) {
        printf("%c", c);
    }
}
