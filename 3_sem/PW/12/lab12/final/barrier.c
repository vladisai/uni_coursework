#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "err.h"

#define N 9
#define K 3
#define MAX_NAP 3
#define LIST_LEN 1000
#define REQUEST_PERMISSION SIGRTMIN
#define GRANT_PERMISSION SIGRTMAX

int requestsSoFar = 0;
int suspended[K];
int signalsReceived = 0;
int signalsList[LIST_LEN];
pid_t parent_pid;

void sleepRandom() {
    int seconds = rand() % MAX_NAP + 1;
    sleep(seconds);
}

int getRandomSignal() {
    int lo = SIGRTMIN + 1;
    int hi = SIGRTMAX - 1;
    int diff = hi - lo;
    int rnd = rand() % (diff + 1);
    int ret = lo + rnd;
    return ret;
}

void setCatch(int signal, void (*catch)(int, siginfo_t *, void *)) {
    struct sigaction action;
    sigset_t block_mask;

    sigemptyset(&block_mask);
    action.sa_sigaction = catch;
    action.sa_mask = block_mask;
    action.sa_flags = SA_SIGINFO | SA_RESTART;

    if (sigaction(signal, &action, 0) == -1) {
        syserr("sigaction");
    }
}

void wakeAll() {
    for (int i = 0; i < K; i++) {
        if (kill(suspended[i], GRANT_PERMISSION) == -1) {
            syserr("kill wakeAll");
        }
    }
    printf("%d woke everyone as the barrier is breached\n", getpid());
}

void catchParentBarrierDown(int sig, siginfo_t *info, void *more) {
    printf("%d received request permission signal (%s) from %d\n", getpid(),
           strsignal(sig), info->si_pid);
    printf("The barrier is down, letting through\n");
    if (kill(info->si_pid, GRANT_PERMISSION) == -1) {
        syserr("kill barrier down");
    }
}

void catchParentBarrierUp(int sig, siginfo_t *info, void *more) {
    printf("%d received request permission signal (%s) from %d\n", getpid(),
           strsignal(sig), info->si_pid);
    suspended[requestsSoFar] = info->si_pid;
    requestsSoFar++;
    if (requestsSoFar == K) {
        printf("Barrier breached\n");
        setCatch(REQUEST_PERMISSION, catchParentBarrierDown);
        wakeAll();
    } else {
        printf("The barrier is still up (%d / %d), putting the process into "
               "the queue\n",
               requestsSoFar, K);
    }
}

void catchParent(int sig, siginfo_t *info, void *more) {
    printf("%d received signal (%s) from %d\n", getpid(), strsignal(sig),
           info->si_pid);
    printf("Adding to the list\n");
    signalsList[signalsReceived++] = sig;
}

void setCatchForAll() {
    for (int sig = SIGRTMIN + 1; sig != SIGRTMAX; sig++) {
        setCatch(sig, catchParent);
    }
}

void parent() {
    printf("Parent\n");

    setCatchForAll();
    setCatch(REQUEST_PERMISSION, catchParentBarrierUp);

    printf("waiting\n");
    for (int i = 0; i < N; i++) {
        if (wait(0) == -1) {
            syserr("wait");
        }
        printf("%d out of %d children have finished\n", i + 1, N);
    }

    printf("Parent received %d signals:\n", signalsReceived);
    for (int i = 0; i < signalsReceived; i++) {
        printf("%s\n", strsignal(signalsList[i]));
    }
}

void catchGrantPermission(int sig, siginfo_t *info, void *more) {
    printf("%d received granted permission signal (%s) from %d\n", getpid(),
           strsignal(sig), info->si_pid);
}

void child() {
    srand(time(NULL) ^
          (getpid() << 16)); // random must be different for different children
    printf("%d child of %d\n", getpid(), parent_pid);
    sleepRandom(); // fake work
    printf("child %d finished preparation\n", getpid());
    
    sigset_t mask, oldMask;
    sigemptyset (&mask);
    sigaddset (&mask, GRANT_PERMISSION);

    if (sigprocmask (SIG_BLOCK, &mask, &oldMask) == -1) { // block GRANT_PERMISSION
        syserr("sigprocmask");
    }

    setCatch(GRANT_PERMISSION, catchGrantPermission);
    if (kill(parent_pid, REQUEST_PERMISSION) == -1) {
        syserr("kill request permission");
    }

    if (sigsuspend (&oldMask) == -1) {
        if (errno != EINTR) {
            syserr("sigsuspend");
        }
    }

    if (sigprocmask (SIG_UNBLOCK, &mask, NULL) == -1) {
        syserr("sigprocmask clean up");
    }

    printf("%d continuing to send message\n", getpid());

    int message = getRandomSignal();
    printf("%d sending %d to parent\n", getpid(), message);
    if (kill(parent_pid, message) == -1) {
        syserr("kill parent message");
    }
    printf("child %d is finished, exiting \n", getpid());
    exit(0);
}

int main() {
    parent_pid = getpid();

    for (int i = 0; i < N; i++) {
        switch (fork()) {
        case -1:
            syserr("Error in fork\n");
            break;
        case 0:
            child();
            break;
        default:
            break;
        }
    }

    if (getpid() == parent_pid) {
        parent();
    }
}
