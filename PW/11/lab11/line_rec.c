#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NR_PROC 5

#define STDIN_DESC 0
#define STDOUT_DESC 1

const int zero = 0;

int substitute(int desc, int target) {
    int k = dup(target);
    if (k == -1) {
        syserr("Error in dup");
    }
    if (close(target) == -1) {
        syserr("Error in close target");    
    }
    if (dup2(desc, target) == -1) {
        syserr("Error in dup2");
    }
    if (close(desc) == -1) {
        syserr("Error in close desc");    
    }
    return k;
}

void replace(int desc, int target) {
    if (close(target) == -1) {
        syserr("Error in close target");    
    }
    if (dup2(desc, target) == -1) {
        syserr("Error in dup2");
    }
    if (close(desc) == -1) {
        syserr("Error in close desc");    
    }
}

void readAndPrint() {
    char buff[300];
    while (fgets(buff, sizeof(buff), stdin)) {
        buff[strlen(buff) - 1] = '\0'; // the newline symbol must be removed
        if (strlen(buff) == 0) break;
        puts(buff);
    }
}

void printOrBranch(int d, int readFrom) {
    char message[100];
    sprintf(message, "Happy holidays from %d", d);

    if (d > 0) {
        // we're not the last ones, so we should fork
        int pipe_dsc[2];
        if (pipe(pipe_dsc) == -1) {
            syserr("Error in pipe");
        }
        int pid;
        switch (pid = fork()) {
        case -1:
            syserr("Error in fork\n");
            break;

        case 0:
            printOrBranch(d - 1, pipe_dsc[0]);
            break;

        default: {
            int out = substitute(pipe_dsc[1], STDOUT_DESC);
            if (readFrom != -1) {
                int in = substitute(readFrom, STDIN_DESC);
                readAndPrint();
                replace(in, STDIN_DESC);
            }
            printf("%s\n", message);
            printf("\n");
            fflush(stdout);
            replace(out, STDOUT_DESC);
            if (wait(0) == -1) {
                syserr("Error in wait");
            }
        }
        }
    } else {
        if (readFrom != -1) {
            int in = substitute(readFrom, STDIN_DESC);
            readAndPrint();
            replace(in, STDIN_DESC);
        }
        puts(message);
    }
}

int main() {
    printOrBranch(NR_PROC, -1);
    return 0;
}
