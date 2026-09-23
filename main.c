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
    uint32_t temp = 0;
    uint32_t i = 0;

    if(n == 0) {
        return 0;
    }

    for(i = 0; i < n-1; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }

    return b;
}

uint32_t strToInt(const char* str) {
    uint32_t result = 0;

    while(*str != '\0') {
        if(*str < '0' || *str > '9') {
            return result;
        }
        result *= 10;
        result += *str - '0';
        str++;
    }

    return result;
}

int main(int argc, char** argv) {

    if(argc == 1 || argc-1 > MAX_ARGS) {
        printf("Usage: ./main (x0) [x1] [x2] ... [x%d]\n",MAX_ARGS-1);
        return -1;
    }

    __pid_t children[MAX_ARGS] = {0};
    int pipes[2][MAX_ARGS] = {0};

    for(int i = 1; i < argc; i++) {
        const char* thisArg = argv[i];
        uint32_t thisInt = strToInt(thisArg);

        if(pipe(pipes[i-1]) < 0) {
            printf("Could not create pipe\n");
            exit(-1);
        }
        children[i-1] = fork();

        if(children[i-1] < 0) {
            printf("Could not create fork\n");
            exit(children[i-1]);

        } else if (children[i-1] == 0) { //child
            char buf[64];
            sprintf(buf,"%d",fib(thisInt));
            if(write(pipes[i-1][1],buf,strlen(buf)+1)!=strlen(buf)+1) {
                exit(-1);
            }
            exit(0);
        }
    }

    // parent
    for(int i = 0; i < argc-1; i++) {
        int stat = 0;
        waitpid(children[i],&stat,0);
        if(stat != 0) {
            printf("Child %d failed with code %d\n",i,stat);
        }
        char buf[64];
        if(read(pipes[i][0],&buf,sizeof(buf)) == 0) {
            printf("Could not read from pipe %d.\n",i);
        } else {
            printf("Fibonacci of %s is %s\n",argv[i+1],buf);
        }
    }

    return 0;
}
