#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 8

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

    if(argc == 1 || argc-1 > MAX_ARGS) {
        return -1;
    }

    __pid_t children[MAX_ARGS] = {0};
    int pipes[2][MAX_ARGS];
    for(int i = 1; i < argc; i++) {
        const char* thisArg = argv[i];
        uint32_t thisInt = strToInt(thisArg);
        pipe(pipes[i-1]);
        children[i-1] = fork();
        if(children[i-1] < 0) {
            printf("Could not create fork\n");
            exit(children[i-1]);
        } else if (children[i-1] == 0) { //child
            char buf[64];
            sprintf(buf,"%d",fib(thisInt));
            if(write(pipes[i-1][1],buf,strlen(buf)+1)!=strlen(buf)+1) {
                printf("couldnt write!\n");
                exit(-1);
            } else {
                //printf("wrote %s to pipe!\n",buf);
            }
            exit(0);
        }
    }

    for(int i = 0; i < argc-1; i++) {
        int stat = 0;
        waitpid(children[i],&stat,0);
        char buf[64];
        if(read(pipes[i][0],&buf,sizeof(buf)) == 0) {
            printf("couldnt read from pipe!\n");
        } else {
            printf("Fibonacci of %s is %s\n",argv[i+1],buf);
        }
    }

    return 0;
}
