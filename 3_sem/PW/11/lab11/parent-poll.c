#include "err.h"
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define MAX_ENTRIES 2
#define stdin_dsc 0

int main(int argc, char *argv[]) {
    int pipe_dsc[2], buf_len, finished, ret, i;
    char buf[BUF_SIZE];
    struct pollfd entries[MAX_ENTRIES];

    if (pipe(pipe_dsc) == -1)
        syserr("Error in pipe\n");

    switch (fork()) {
    case -1:
        syserr("Error in fork\n");

    case 0:
        if (dup2(pipe_dsc[1], 1) != 1)
            syserr("Error in child, dup2\n");
        if (close(pipe_dsc[0]) == -1)
            syserr("Error in child, close (pipe_dsc[0])\n");
        if (close(pipe_dsc[1]) == -1)
            syserr("Error in child, close (pipe_dsc[1])\n");

        execl("./child-poll", "child-poll", NULL);
        syserr("Error in execl\n");

    default:
        if (close(pipe_dsc[1]) == -1)
            syserr("Error in parent, close (pipe_dsc[1])\n");
    }

    for (i = 0; i < MAX_ENTRIES;
         ++i) /* rodzic będzie czekał na dane do odczytu */
        entries[i].events = POLLIN;
    entries[0].fd = stdin_dsc;   /* ze standardowego wejścia */
    entries[1].fd = pipe_dsc[0]; /* od potomka */
    finished = 0;
    printf("Type something (CTRL-D to exit)\n");

    while (!finished) {

        for (i = 0; i < MAX_ENTRIES;
             ++i) /* zerowanie revents jest konieczne przed każdym poll */
            entries[i].revents = 0;

        if ((ret = poll(entries, MAX_ENTRIES, -1)) ==
            -1) /* czekanie na wielu deskryptorach */
            syserr("Error in poll\n");
        else if (ret > 0)
            for (i = 0; i < MAX_ENTRIES; ++i)
                if (entries[i].revents &
                    (POLLIN | POLLERR)) { /* sprawdzenie czy można czytać */

                    if ((buf_len = read(entries[i].fd, buf, BUF_SIZE - 1)) ==
                        -1)
                        syserr("Error in read\n");

                    if ((i == stdin_dsc) &&
                        (buf_len ==
                         0)) { /* zamknięcie std wejścia oznacza koniec */
                        printf("Goodbye\n");
                        finished = 1;
                    } else { /* wypisanie danych */
                        buf[buf_len < BUF_SIZE - 1 ? buf_len : BUF_SIZE - 1] =
                            '\0';
                        printf("Read %d byte(s) from descriptor %d: %s\n",
                               buf_len, entries[i].fd, buf);
                    }
                }
    }

    if (close(pipe_dsc[0]) == -1)
        syserr("Error in parent, close (pipe_dsc[0])\n");

    if (wait(0) == -1)
        syserr("Error in wait\n");

    exit(0);
}
