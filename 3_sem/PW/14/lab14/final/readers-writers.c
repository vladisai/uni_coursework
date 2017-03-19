#include "err.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READERS 2
#define WRITERS 3
#define NAP 20
#define BSIZE 32
#define MAX_NAP 5

struct readwrite_t {
    pthread_mutex_t lock;
    pthread_cond_t readers;
    pthread_cond_t writers;
    int rcount, wcount, rwait, wwait;
    int change;
};

struct readwrite_t library;
char book[BSIZE];
int working = 1;

void cond_wait(pthread_cond_t *c, pthread_mutex_t *l) {
    int err;
    if ((err = pthread_cond_wait(c, l)) != 0)
        syserr(err, "cond wait failed");
}

void cond_signal(pthread_cond_t *c) {
    int err;
    if ((err = pthread_cond_signal(c)) != 0) {
        syserr(err, "cond signal failed");
    }
}

void lock(pthread_mutex_t *l) {
    int err;
    if ((err = pthread_mutex_lock(l)) != 0)
        syserr(err, "lock failed");
}

void unlock(pthread_mutex_t *l) {
    int err;
    if ((err = pthread_mutex_unlock(l)) != 0)
        syserr(err, "unlock failed");
}

void sleepRandom() { sleep(rand() % MAX_NAP + 1); }

void printLibraryState() {
    pthread_t tid = pthread_self() % 100;
    printf("thread %lu : rcount %d wcount %d rwait %d wwait %d change %d\n",
           tid, library.rcount, library.wcount, library.rwait, library.wwait,
           library.change);
}

/* Initialize a buffer */

void init(struct readwrite_t *rw) {
    int err;
    if ((err = pthread_mutex_init(&rw->lock, 0) != 0))
        syserr(err, "mutex init failed");
    if ((err = pthread_cond_init(&rw->readers, 0)) != 0)
        syserr(err, "cond init readers failed");
    if ((err = pthread_cond_init(&rw->writers, 0)) != 0)
        syserr(err, "cond init writers failed");
    rw->rcount = 0;
    rw->wcount = 0;
    rw->rwait = 0;
    rw->wwait = 0;
    rw->change = 0;
}

/* Destroy the buffer */

void destroy(struct readwrite_t *rw) {
    int err;
    if ((err = pthread_cond_destroy(&rw->readers)) != 0)
        syserr(err, "cond destroy readers failed");
    if ((err = pthread_cond_destroy(&rw->writers)) != 0)
        syserr(err, "cond destroy writers failed");
    if ((err = pthread_mutex_destroy(&rw->lock)) != 0)
        syserr(err, "mutex destroy failed");
}

void *reader(void *data) {
    pthread_t tid = pthread_self() % 100;
    while (working) {
        lock(&library.lock);

        printf("reader %lu: ", tid);
        printLibraryState();

        library.rwait++;

        while (library.wcount || (library.change <= 0 && library.wwait)) {
            printf("reader %lu has to wait for writer to finish\n", tid);
            cond_wait(&library.readers, &library.lock);
            printf("reader %lu was woken up\n", tid);
        }
        if (library.change > 0)
            library.change--;

        unlock(&library.lock);

        printf("reader %lu has entered cs\n", tid);

        library.rwait--;
        library.rcount++;

        sleepRandom();
        printf("reader %lu read: %s\n", tid, book); /* reading */

        lock(&library.lock);
        library.rcount--;
        if (library.rcount == 0 && library.wwait) {
            printLibraryState();
            printf("reader %lu is about to wake up a writer\n", tid);
            library.change = -1;
            cond_signal(&library.writers);
        }
        unlock(&library.lock);
    }
    return 0;
}

void *writer(void *data) {
    int l;
    pthread_t tid = pthread_self() % 100;
    printf("writer %lu started\n", tid);
    while (working) {
        lock(&library.lock);

        printf("writer %lu: ", tid);
        printLibraryState();

        library.wwait++;
        while (library.rcount || library.wcount ||
               (library.change >= 0 && library.rwait)) {
            printf("%lu waiting for writer signal\n", tid);
            cond_wait(&library.writers, &library.lock);
            printf("%lu writer was woken up\n", tid);
        }
        if (library.change < 0)
            library.change++;
        printf(" writer %lu entered cs\n", tid);
        library.wcount++;
        library.wwait--;

        unlock(&library.lock);

        l = rand() % 10;
        snprintf(book, BSIZE, "6 times a number %d %d %d %d %d %d", l, l, l, l,
                 l, l);
        sleepRandom();
        printf("writer %lu wrote: %s\n", tid, book); /* reading */

        lock(&library.lock);

        library.change = 0;
        library.wcount--;

        printLibraryState();
        if (library.rwait == 0 && library.wwait) {
            printf("%lu wants to wake up next writer as there are no readers\n",
                   tid);
            cond_signal(&library.writers);
        } else if (library.rwait > 0) {
            printf("%lu wants to wake up all the waiting readers\n", tid);
            library.change = library.rwait;
            for (int i = 0; i < library.rwait; i++) {
                cond_signal(&library.readers);
            }
        }
        unlock(&library.lock);
    }
    return 0;
}

int main() {
    pthread_t th[READERS + WRITERS];
    pthread_attr_t attr;
    int err;
    void *retval;

    srand((unsigned)time(0));

    init(&library);

    /* creating threads */

    if ((err = pthread_attr_init(&attr)) != 0)
        syserr(err, "attr_init failed");
    if ((err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) !=
        0)
        syserr(err, "attr_setdetachstate failed");

    for (int i = 0; i < READERS; i++) {
        if ((err = pthread_create(&th[i], &attr, reader, 0)) != 0)
            syserr(err, "create reader failed");
    }
    for (int i = 0; i < WRITERS; i++) {
        if ((err = pthread_create(&th[READERS + i], &attr, writer, 0)) != 0)
            syserr(err, "create writer failed");
    }

    sleep(NAP);
    working = 0;

    /* waiting for threads */

    for (int i = 0; i < READERS + WRITERS; i++) {
        if ((err = pthread_join(th[i], &retval)) != 0)
            syserr(err, "join %d failed", i);
    }

    if ((err = pthread_attr_destroy(&attr)) != 0)
        syserr(err, "attr destroy failed");

    destroy(&library);

    return 0;
}
