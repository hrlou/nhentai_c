#include <stdio.h>
#include <stdlib.h>

struct sex {
    char* penis;
    char* cock;
};

void main(void) {
    struct sex lol;

    char** arrop[2];

    arrop[0] = &lol.penis;
    arrop[1] = &lol.cock;

    *arrop[0] = "sex";
    *arrop[1] = "vagina";

    for (int i = 0; i < 2; i++) {
        printf("Address = %d\t Value = %s\n", arrop[i], *arrop[i]);
    }

    char** test;
    test = (char **)calloc(2, sizeof(char*));
    test[0] = "hello";
    test[1] = "sex";

    for (int i = 0; i < 2; i++) {
        printf("Value = %s\n", test[i]);
    }
}