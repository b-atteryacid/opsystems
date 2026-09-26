#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_ARGS 8
#define STR_BUF_SIZE 16

/* @brief Return the nth Fibonacci number.
 * @param n The index of the number to return.
 * @return The nth Fibonacci number.
 */
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

/* @brief Convert a string to a positive integer.
 * @param str Pointer to the string. Must be null-terminated and contain a positive integer.
 * @return The integer representation in the string.
 */
uint32_t strToInt(const char* str) {
    uint32_t result = 0;

    while(*str != '\0') {
        /*handle non-digits*/
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
    int children[MAX_ARGS];
    int pipes[MAX_ARGS][2];
    const char* thisArg;
    uint32_t thisInt;
    int i;
    int stat;
    char buf[STR_BUF_SIZE];

    if(argc == 1 || argc-1 > MAX_ARGS) {
        printf("Usage: ./main (n0) [n1] [n2] ... [n%d]\n",MAX_ARGS-1);
        return -1;
    }

    /*Spawn children and setup pipes*/
    for(i = 1; i < argc; i++) {
        thisArg = argv[i];
        thisInt = strToInt(thisArg);

        if(pipe(pipes[i-1]) < 0) {
            printf("Could not create pipe\n");
            exit(-1);
        }

        children[i-1] = fork();

        /*error*/
        if(children[i-1] < 0) {
            printf("Could not create fork\n");
            exit(children[i-1]);

        }
        /*child*/
        else if (children[i-1] == 0) {
            sprintf(buf,"%d",fib(thisInt));
            if(write(pipes[i-1][1],buf,strlen(buf)+1) != strlen(buf)+1) {
                exit(-1);
            }
            exit(0);
        }
    }

    /*Parent only, wait and read from pipes*/
    for( i = 0; i < argc-1; i++) {
        stat = 0;
        waitpid(children[i],&stat,0);
        if(stat != 0) {
            printf("Child %d failed with code %d\n",i,stat);
        }
        if(read(pipes[i][0],buf,STR_BUF_SIZE) == 0) {
            printf("Could not read from pipe %d.\n",i);
        } else {
            printf("Fibonacci of %s is %s\n",argv[i+1],buf);
        }
    }

    return 0;
}
