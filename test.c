#include <stdio.h>
#include <stdlib.h>

struct sex {
    char* penis;
    char* cock;
};

void main(void) {
    struct sex lol;
    const int (&point[]) = {
        *lol.penis,
        *lol.cock
    };
    int 
    point[0] = "sex";

    printf("%s\n", lol.penis);
} 
