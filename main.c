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

    for(i = 0; i < n; i++) {
        c = a + b;
        a = b;
        b = c;
    }

    return b;

}

int main() {
    uint32_t i = 0;
    printf("test\n");

    for(i = 0; i < 100; i++) {
        printf("fib %d: %d\n",i,fib(i));
    }


    return 0;
}
