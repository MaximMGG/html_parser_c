#include <string.h>
#include <stdlib.h>
#include <iostream>



struct b {
    char *res;
    size_t size;
};


int main() {

    struct b m;
    m.res = (char *) malloc(sizeof(char) * 100);


    std::cout << strlen(m.res);

    free(m.res);

    return 0;
}
