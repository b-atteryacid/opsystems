#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

    __pid_t children[8] = {0};
    for(int i = 1; i < argc; i++) {
        const char* thisArg = argv[i];
        printf("got argument: %s\n",thisArg);
        uint32_t thisInt = strToInt(thisArg);
        children[i-1] = fork();
        if(children[i-1] < 0) {
            printf("Could not create fork\n");
            exit(children[i-1]);
        } else if (children[i-1] == 0) { //child
            printf("child calculating %d\n",thisInt);
            exit(fib(thisInt));
        }
    }

    for(int i = 0; i < sizeof(children)/sizeof(children[0]); i++) {
        int stat = 0;
        waitpid(children[i],&stat,0);

    }

    return 0;
}
