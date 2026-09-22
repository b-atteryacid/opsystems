#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

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
    int pipes[2][8];
    for(int i = 1; i < argc; i++) {
        const char* thisArg = argv[i];
        printf("got argument: %s\n",thisArg);
        uint32_t thisInt = strToInt(thisArg);
        pipe(pipes[i-1]);
        children[i-1] = fork();
        if(children[i-1] < 0) {
            printf("Could not create fork\n");
            exit(children[i-1]);
        } else if (children[i-1] == 0) { //child
            printf("child calculating %d\n",thisInt);
            printf("fib(%d) is %d\n",thisInt,fib(thisInt));
            if(write(pipes[i-1][1],"test",strlen("test"))!=strlen("test")) {
                printf("couldnt write!\n");
                exit(-1);
            } else {
                printf("wrote to pipe!\n");
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
            printf("read %s from pipe!\n",buf);
        }
        printf("child %d status is %d\n",i,stat);
    }

    return 0;
}
