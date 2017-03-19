#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NR_PROC 5

void foo(int d, int readFrom) {
    printf("foo %d %d\n", d, readFrom);

    char message[100];
    sprintf(message, "Happy holidays from %d", d);

    if (d > 0) {
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
            printf("I am a child %d, going further\n", d);
            foo(d - 1, pipe_dsc[0]);
            break;

        default:
            printf("I am a parent %d, I've just forked\n", d);
            if (readFrom != -1) {
                char buff[300];
                while (read(readFrom, buff, sizeof(int))) {
                    // reading size of the next message
                    int sz;
                    memcpy(&sz, buff, sizeof(int));
                    printf("next size is %d\n", sz);
                    if (sz == 0) {
                        break;
                    } else {
                        read(readFrom, buff, sz);
                        printf("Just read %s\n", buff);
                        write(pipe_dsc[1], &sz, 4);
                        write(pipe_dsc[1], buff, sz);
                    }
                }
                if (close(readFrom)) {
                    syserr("Error in close");
                }
            }
            int sz = strlen(message);
            write(pipe_dsc[1], &sz, 4);
            write(pipe_dsc[1], message, sizeof(message));
            close(pipe_dsc[1]);
            break;
        }
    } else {
        if (readFrom != -1) {
            char buff[300];
            while (read(readFrom, buff, sizeof(int))) {
                // reading size of the next message
                int sz;
                memcpy(&sz, buff, sizeof(int));
                printf("next size is %d\n", sz);
                if (sz == 0) {
                    break;
                } else {
                    read(readFrom, buff, sz);
                    printf("Just read %s\n", buff);
                    puts(buff);
                }
            }
            if (close(readFrom)) {
                syserr("Error in close");
            }
        }
        printf("%s", message);
    }
}

int main() {

    foo(NR_PROC, -1);

    return 0;
}
