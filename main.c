#include <stdio.h>
#include <stdint.h>

uint32_t fib(uint32_t n) {
    uint32_t a = 0;
    uint32_t b = 1;
    uint32_t i = 0;
    uint32_t c = 0;

    if(n == 0) {
        return 0;
    }

    for(i = 0; i < n-1; i++) {
        c = a + b;
        a = b;
        b = c;
    }

    return b;

}

uint32_t strToInt(const char* s) {
    uint32_t r = 0;
    while(*s != '\0') {
        r *= 10;
        r += *s - '0';
        s++;
    }
    return r;
}

int main(int argc, char** argv) {

    if(argc == 1) {
        return -1;
    }
    for(int i = 1; i < argc; i++) {
        const char* thisArg = argv[i];
        printf("got argument: ");
        printf(thisArg);
        uint32_t thisInt = strToInt(thisArg);
        printf("\nthat is %d in int!!\n",thisInt);
        printf("fib %d: %d\n",thisInt,fib(thisInt));
    }

    return 0;
}
