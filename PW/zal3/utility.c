#include "utility.h"
#include "list.h"
#include "err.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


int parseNumber(char *str, int *ind) {
    int val = 0;
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
    time = 1;

    for (int i = 0; i < MAX_VARS; i++) {
        if (isInCircuit[i] && !isVisited[i]) {
            if (dfs(i)) {
                return 1;
            }
        }
    }
    return 0;
}

int *parseValues(char *str) {
    int len = strlen(str);
    int *ret = (int *)malloc(sizeof(int) * MAX_VARS);
    for (int i = 0; i < MAX_VARS; i++)
        ret[i] = INF;

    for (int i = 0; i < len; i++) {
        i += 2;
        int var = parseNumber(str, &i);
        i += 3;
        int val = parseNumber(str, &i);
        i++;
        ret[var] = val;
    }
    return ret;
}

int checkReachability(int *vals, int x) {
    if (vals[x] != INF) {
        return 1;
    }
    list_ptr cur = graph[x];
    if (cur == 0 && vals[x] == INF) {
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

int checkExistence(int *vals, int len) {
    for (int i = 0; i < len; i++) {
        if (vals[i] != INF && isInCircuit[i] == 0) {
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

void readInt(int readDescriptor, int *value) {
    if (read(readDescriptor, value, sizeof(int)) != sizeof(int)) {
        syserr("read from %d\n", readDescriptor);
    }
}
