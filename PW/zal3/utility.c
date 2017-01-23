#include "utility.h"
#include "err.h"
#include "list.h"
#include "message.h"
#include "node.h"

#include <ctype.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long parseNumber(char *str, int *ind) {
    long val = 0;
    int l = strlen(str);
    while (*ind < l && isdigit(str[*ind])) {
        val *= 10;
        val += str[*ind] - '0';
        (*ind)++;
    }
    (*ind)--;
    return val;
}

void addEdges(int x, char *s) {
    int l = strlen(s);
    for (int i = 0; i < l; i++) {
        if (s[i] == '[') {
            i++;
            int e = parseNumber(s, &i);
            isInCircuit[e] = 1;
            addInt(&graph[x], e);
        }
    }
}

int dfs(int x) {
    isVisited[x] = 1;
    isInStack[x] = 1;
    list_ptr cur = graph[x];
    while (cur != 0) {
        int e = getTopInt(cur);
        if (!isVisited[e] && dfs(e))
            return 1;
        if (isInStack[e])
            return 1;
        cur = cur->tail;
    }
    isInStack[x] = 0;
    return 0;
}

int checkCycles() {
    memset(isVisited, 0, sizeof(int) * MAX_VARS);
    memset(isInStack, 0, sizeof(int) * MAX_VARS);

    for (int i = 0; i < MAX_VARS; i++) {
        if (isInCircuit[i] && !isVisited[i]) {
            if (dfs(i)) {
                return 1;
            }
        }
    }
    return 0;
}

long *parseValues(char *str) {
    int len = strlen(str);
    long *ret = (long*)malloc(sizeof(long) * MAX_VARS);
    for (int i = 0; i < MAX_VARS; i++)
        ret[i] = LINF;

    for (int i = 0; i < len; i++) {
        i += 2;
        long var = parseNumber(str, &i);
        i += 3;
        long val = parseNumber(str, &i);
        i++;
        if (ret[var] != LINF) {
            free(ret);
            return 0;
        }
        ret[var] = val;
    }
    return ret;
}

int checkReachability(long *vals, int x) {
    if (vals[x] != LINF) {
        return 1;
    }
    list_ptr cur = graph[x];
    if (cur == 0 && vals[x] == LINF) {
        fprintf(stderr, "reached %d, it's not evaluated\n", x);
        return 0;
    }
    while (cur != 0) {
        if (!checkReachability(vals, getTopInt(cur))) {
            return 0;
        }
        cur = cur->tail;
    }
    return 1;
}

int checkExistence(long *vals, int len) {
    for (int i = 0; i < len; i++) {
        if (vals[i] != LINF && isInCircuit[i] == 0) {
            fprintf(stderr, "%d isn't in the circuit but is evaluated\n", i);
            return 0;
        }
    }
    return 1;
}

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
    writeInt(writeDescriptor, m->type);
    writeInt(writeDescriptor, m->init_id);
    writeInt(writeDescriptor, m->value);
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
    int type, init_id, value;
    type = readInt(readDescriptor);
    init_id = readInt(readDescriptor);
    value = readInt(readDescriptor);
    return createMessage(type, init_id, value);
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

int isOperation(char c) {
    if (c == '*')
        return 1;
    else if (c == '+')
        return 1;
    else if (c == '-')
        return 1;
    return 0;
}

int getPriority(char c) {
    if (c == '*')
        return 2;
    else if (c == '+')
        return 1;
    else if (c == '-')
        return 3;
    else
        return -1;
}

void performOne(list_ptr *vals, list_ptr *ops) {
    char c = popAndGetInt(ops);
    if (c == '-') {
        node_ptr val = popAndGetNode(vals);
        addNode(vals, createUnaryOperationNode('-', val));
    } else if (c == '*') {
        node_ptr valA = popAndGetNode(vals);
        node_ptr valB = popAndGetNode(vals);
        addNode(vals, createBinaryOperationNode('*', valA, valB));
    } else if (c == '+') {
        node_ptr valA = popAndGetNode(vals);
        node_ptr valB = popAndGetNode(vals);
        addNode(vals, createBinaryOperationNode('+', valA, valB));
    }
}

node_ptr build(char *str) {
    int len = strlen(str);
    list_ptr vals = createEmptyList();
    list_ptr ops = createEmptyList();
    for (int i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            if (isOperation(str[i])) {
                while (ops != 0 &&
                       getPriority(getTopInt(ops)) > getPriority(str[i])) {
                    performOne(&vals, &ops);
                }
                addInt(&ops, str[i]);
            } else if (str[i] == '(') {
                addInt(&ops, str[i]);
            } else if (str[i] == ')') {
                while (getTopInt(ops) != '(') {
                    performOne(&vals, &ops);
                }
                ops = popInt(ops);
            } else if (isdigit(str[i])) {
                int x = parseNumber(str, &i);
                addNode(&vals, createValueNode(x));
            } else if (str[i] == 'x') {
                i += 2;
                int x = parseNumber(str, &i);
                i++;
                addNode(&vals, getOrCreateVariableNode(x));
            }
        }
    }
    while (ops != 0) {
        performOne(&vals, &ops);
    }
    return popAndGetNode(&vals);
}
