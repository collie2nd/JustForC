#include <stdio.h>
#include <string.h>

int main() {
    unsigned char a = 1;
    char b[32] = {0};

    memset(b, 0, sizeof(b));
    strcpy(b, "true");

    a = (a) ? 1 : 0;

    printf("%s\n", b);
    printf("a = %d\n", a);
    return 0;
}