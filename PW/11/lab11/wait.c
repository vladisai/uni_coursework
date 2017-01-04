#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define STDIN_DESC 0
#define STDOUT_DESC 1

int substitute(int desc, int target) {
    int k = dup(target);
    close(target);
    dup2(desc, target);
    close(desc);
    return k;
}

void replace(int desc, int target) {
    close(target);
    dup2(desc, target);
    close(desc);
}

void logg(const char *str) {
    fprintf(stderr, "%s\n", str);
}

void readAndPrint() {
    logg("readAndPrint");
    char buff[300];
    while (fgets(buff, sizeof(buff), stdin)) {
        logg("inside");
        buff[strlen(buff) - 1] = '\0'; // the newline symbol must be removed
        if (strlen(buff) == 0) break;
        puts(buff);
    }
}

int main() {
    pid_t pid;
    char msg[] = "123";

    int pipe_dsc[2];
    if (pipe(pipe_dsc) == -1) {
        syserr("Error in pipe");
    }
    switch (pid = fork()) { /* powstaje nowy proces */

    case -1: /* błąd funkcji fork() */
        syserr("Error in fork\n");
        break;

    case 0: /* proces potomny */
        printf("I am a child and my pid is %d\n", getpid());
        printf("I am a child and fork() return value is %d\n", pid);

        int b = substitute(pipe_dsc[0], 0);
        readAndPrint();
        replace(b, 0);

        /* proces potomny kończy */
        break;

    default: /* proces macierzysty */

        printf("I am a parent and my pid is %d\n", getpid());
        printf("I am a parent and fork() return value is %d\n", pid);

        int out = substitute(pipe_dsc[1], STDOUT_DESC);

        printf("%s\n", msg);
        printf("%s\n", msg);
        printf("\n");

        puts("OKK");

        replace(out, 1);
        puts("OK");
        
        if (wait(0) == -1)
            syserr("Error in wait\n");
        /* czeka na zakończenie procesu potomnego */

        return 0;
    }
}
