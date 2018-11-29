#include <stdio.h>

int main() {
    short x = 1;
    char c = *(char *)&x;
    if (c==1){
        printf("little endian\n");
    } else{
        printf("big endian\n");
    }
    return 0;
}