#ifndef UTILITY_H
#define UTILITY_H

#include "list.h"

#define MAX_VARS 1000
#define MAX_NODES 10000
#define EXPR_LEN 2000
#define INF (1 << 30)

list_ptr graph[MAX_VARS];
int isInCircuit[MAX_VARS];
int isVisited[MAX_VARS];
int isInStack[MAX_VARS];
int time;

int parseNumber(char *str, int *ind);

void addEdges(int x, char *s);

int dfs(int x);

int checkCycles();

int *parseValues(char *str);

int checkReachability(int *vals, int x);

int checkExistence(int *vals, int len);

void createPipe(int *read, int *write);

void writeInt(int writeDescriptor, int value);

void readInt(int readDescriptor, int *value);

#endif // UTILITY_H
