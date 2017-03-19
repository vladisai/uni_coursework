#include "err.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PRODUCERS_NUMBER 2
#define CONSUMERS_NUMBER 2
#define PRODUCTS_PER_PRODUCER 3
#define MAX_NAP 2
#define BUFFER_SIZE 3
#define MEMORY_SIZE BUFFER_SIZE + 8 + 2 * sizeof(sem_t)
#define MAX_CONSUMED PRODUCERS_NUMBER *PRODUCTS_PER_PRODUCER

sem_t *writeSemaphore;
sem_t *readSemaphore;
sem_t *bufferMutex;
sem_t *totalConsumedMutex;
int *readIndex;
int *writeIndex;
int *vals;
int *pids;
int *totalConsumed;

void *createSharedOfSize(int size) {
    void *mapped_mem;
    int fd_memory = -1;
    int flags, prot;

    prot = PROT_READ | PROT_WRITE;
    flags = MAP_SHARED | MAP_ANONYMOUS;
    mapped_mem = mmap(NULL, size, prot, flags, fd_memory, 0);

    if (mapped_mem == MAP_FAILED)
        syserr("mmap");

    return mapped_mem;
}

void initShared() {
    int current = 0;
    int semSize = sizeof(sem_t);
    int intSize = sizeof(int);
    int bufferSize = intSize * BUFFER_SIZE;
    int totalSize = 4 * semSize + 3 * intSize + 2 * bufferSize;

    void *start = createSharedOfSize(totalSize);
    writeSemaphore = (sem_t *)(start + current);
    current += semSize;

    if (sem_init(writeSemaphore, 1, BUFFER_SIZE) == -1)
        syserr("sem init write");

    readSemaphore = (sem_t *)(start + current);
    current += semSize;
    if (sem_init(readSemaphore, 1, 0) == -1)
        syserr("sem init read");

    bufferMutex = (sem_t *)(start + current);
    current += semSize;
    if (sem_init(bufferMutex, 1, 1) == -1)
        syserr("sem init mutex");

    totalConsumedMutex = (sem_t *)(start + current);
    current += semSize;
    if (sem_init(totalConsumedMutex, 1, 1) == -1)
        syserr("sem init mutex");

    readIndex = (int *)(start + current);
    current += intSize;
    *readIndex = 0;

    writeIndex = (int *)(start + current);
    current += intSize;
    *writeIndex = 0;

    totalConsumed = (int *)(start + current);
    current += intSize;
    *totalConsumed = 0;

    vals = (int *)(start + current);
    current += bufferSize;

    pids = (int *)(start + current);
    current += bufferSize;
}

int generateProduct() { return rand() % 10; }

void sleepRandom() { sleep(rand() % MAX_NAP + 1); }

void shift(int *i) { *i = (*i + 1) % BUFFER_SIZE; }

void putIntoBuffer(int val, int pid) {
    // printf("waiting write semaphore\n");
    if (sem_wait(writeSemaphore) == -1) {
        syserr("sem wait write");
    }
    // printf("finished waiting write semaphore\n");

    if (sem_wait(bufferMutex) == -1) {
        syserr("sem wait mutex");
    }

    vals[*writeIndex] = val;
    pids[*writeIndex] = pid;

    shift(writeIndex);

    if (sem_post(bufferMutex) == -1) {
        syserr("sem post mutex");
    }

    if (sem_post(readSemaphore) == -1) {
        syserr("sem post read");
    }
    // printf("increased read semaphore\n");
}

void getFromBuffer(int *val, int *pid) {
    if (sem_wait(totalConsumedMutex) == -1) {
        syserr("sem wait consumed");
    }

    int shouldExit = 0;

    if (*totalConsumed == MAX_CONSUMED) {
        shouldExit = 1;
    } else {
        (*totalConsumed)++;
    }

    if (sem_post(totalConsumedMutex) == -1) {
        syserr("sem post consumed");
    }

    if (shouldExit) {
        *val = -1;
        *pid = -1;
        return;
    }

    // printf("waiting read semaphore\n");
    if (sem_wait(readSemaphore) == -1) {
        syserr("sem wait read");
    }
    // printf("finished waiting read semaphore\n");

    if (sem_wait(bufferMutex) == -1) {
        syserr("sem wait mutex");
    }

    *val = vals[*readIndex];
    *pid = pids[*readIndex];

    shift(readIndex);

    if (sem_post(bufferMutex) == -1) {
        syserr("sem post mutex");
    }

    if (sem_post(writeSemaphore) == -1) {
        syserr("sem post write");
    }
}

void consumer() {
    srand(time(NULL) ^ (getpid() << 16));

    sleepRandom();

    printf("Consumer %d started\n", getpid());

    while (1) {
        int val, pid;
        getFromBuffer(&val, &pid);
        if (val == pid && pid == -1)
            break;
        printf("Consumer %d is consuming (%d, %d)\n", getpid(), val, pid);
        sleep(val);
        printf("Consumer %d finished consuming (%d, %d)\n", getpid(), val, pid);
    }
    printf("Consumer %d finished\n", getpid());
}

void producer() {
    srand(time(NULL) ^ (getpid() << 16));

    sleepRandom();

    printf("Producer %d started\n", getpid());
    for (int i = 0; i < PRODUCTS_PER_PRODUCER; i++) {
        int p = generateProduct();
        printf("Producer %d generated and is going to put (%d, %d) into the "
               "buffer\n",
               getpid(), p, getpid());
        putIntoBuffer(p, getpid());
        printf("Producer %d finished putting (%d, %d) into the buffer\n",
               getpid(), p, getpid());
    }
    printf("Producer %d finished\n", getpid());
}

int main() {

    initShared();

    for (int i = 0; i < PRODUCERS_NUMBER; i++) {
        switch (fork()) {
        case 0:
            producer();
            exit(0);
        default: 
            break;
        }
    }

    for (int i = 0; i < CONSUMERS_NUMBER; i++) {
        switch (fork()) {
        case 0:
            consumer();
            exit(0);
        default: 
            break;
        }
    }

    for (int i = 0; i < PRODUCERS_NUMBER + CONSUMERS_NUMBER; i++) {
        if (wait(0) == -1) {
            printf("%d finished\n", i);
        }
    }

    return 0;
}
