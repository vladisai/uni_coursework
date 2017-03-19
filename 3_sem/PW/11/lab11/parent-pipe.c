#include "err.h"
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct message {
    int type;
    long value;
    int isValueUndefined;
    int init_id; // id of the initialization
} message_t, *message_ptr;

message_ptr createMessage(int type, int init_id, long value) {
    message_ptr ret = (message_ptr)malloc(sizeof(message_t));
    ret->init_id = init_id;
    ret->value = value;
    ret->type = type;
    return ret;
}

typedef struct list {
    void *element;
    struct list *tail;
} list_t, *list_ptr;

void addElement(list_ptr *l, void *p) {
    list_t *newList = (list_t *)malloc(sizeof(list_t));
    newList->tail = *l;
    newList->element = p;
    *l = newList;
}

void addInt(list_ptr *l, int e) {
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = e;
    addElement(l, ptr);
}

int getTopInt(list_ptr l) { return *((int *)l->element); }

list_ptr pop(list_ptr l) {
    list_t *ret = l->tail;
    free(l);
    return ret;
}

int popAndGetInt(list_ptr *l) {
    int val = getTopInt(*l);
    *l = pop(*l);
    return val;
}

void printAllInts(list_ptr l) {
    list_ptr p = l;
    printf("[");
    while (p != 0) {
        printf("%d, ", getTopInt(p));
        p = p->tail;
    }
    printf("]");
}

void shift(list_ptr *p) {
    if (*p != 0)
        *p = (*p)->tail;
}

int isEmpty(list_ptr p) { return p == 0; }

int getLen(list_ptr p) {
    int res = 0;
    while (p != 0) {
        res++;
        shift(&p);
    }
    return res;
}

list_ptr createEmptyList() { return 0; }

void createPipe(int *read, int *write) {
    int desc[2];
    if (pipe(desc) == -1) {
        // error log message
    }
    *read = desc[0];
    *write = desc[1];
}

void writeInt(int writeDescriptor, int value) {
    if (write(writeDescriptor, &value, sizeof(int)) != sizeof(int)) {
        syserr("write %d to %d\n", value, writeDescriptor);
    }
}

void writeLong(int writeDescriptor, long value) {
    if (write(writeDescriptor, &value, sizeof(long)) != sizeof(long)) {
        syserr("write %ld to %d\n", value, writeDescriptor);
    }
}

void writeMessage(int writeDescriptor, message_ptr m) {
    if (write(writeDescriptor, &m, sizeof(m)) != sizeof(m)) {
        syserr("write %ld to %d\n", 1, writeDescriptor);
    }
}

int readInt(int readDescriptor) {
    int *value = (int *)malloc(sizeof(int));
    if (read(readDescriptor, value, sizeof(int)) != sizeof(int)) {
        syserr("read from %d\n", readDescriptor);
    }
    return *value;
}

long readLong(int readDescriptor) {
    long *value = (long *)malloc(sizeof(long));
    if (read(readDescriptor, value, sizeof(long)) != sizeof(long)) {
        syserr("read from %d\n", readDescriptor);
    }
    return *value;
}

message_ptr readMessage(int readDescriptor) {
    message_ptr *value = (message_ptr *)malloc(sizeof(message_ptr));
    if (read(readDescriptor, value, sizeof(message_ptr)) != sizeof(message_ptr)) {
        syserr("read from %d\n", readDescriptor);
    }
    return *value;
}

message_ptr readFromAll(list_ptr descriptors) {
    int len = getLen(descriptors);
    struct pollfd entries[len];
    for (int i = 0; i < len; i++) {
        entries[i].events = POLLIN;
        entries[i].fd = getTopInt(descriptors);
        entries[i].revents = 0;
        shift(&descriptors);
    }
    int ret;
    if ((ret = poll(entries, len, -1)) == -1) {
        syserr("Error in poll\n");
    } else if (ret > 0) {
        for (int i = 0; i < len; ++i) {
            if (entries[i].revents & (POLLIN | POLLERR)) {
                printf("read!\n");
                return readMessage(entries[i].fd);
            }
        }
    }
    return 0;
}

void writeToAll(list_ptr descriptors, message_ptr m) {
    while (descriptors != 0) {
        writeMessage(getTopInt(descriptors), m);
        shift(&descriptors);
    }
}

int main() {
    int read, write;
    createPipe(&read, &write);

    int ok = 1;
    long huge = 1LL << 60;
    printf("%ld\n", huge);
    switch (fork()) {
    case -1:
        syserr("Error in fork\n");

    case 0: {

        message_ptr rd = readMessage(read);
        printf("read %d %ld %d\n", rd->init_id, rd->value, rd->type);
        break;
    }
    default: {
        message_ptr rd = createMessage(ok, ok, huge);
        printf("write %d %ld %d\n", rd->init_id, rd->value, rd->type);
        writeMessage(write, rd);
        exit(0);
    }
    }
    return 0;
}
