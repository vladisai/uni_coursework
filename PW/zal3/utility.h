#ifndef UTILITY_H
#define UTILITY_H

#include "list.h"

#define MAX_VARS 1000
#define MAX_NODES 10000
#define EXPR_LEN 2000
#define MAX_OPS 1000
#define INF (1 << 30)

struct message;

list_ptr graph[MAX_VARS];
int isInCircuit[MAX_VARS];
int isVisited[MAX_VARS];
int isInStack[MAX_VARS];

long parseNumber(char *str, int *ind);

void addEdges(int x, char *s);

int dfs(int x);

int checkCycles();

int *parseValues(char *str);

int checkReachability(int *vals, int x);

int checkExistence(int *vals, int len);

void createPipe(int *read, int *write);

void writeMessage(int writeDescriptor, struct message *m);

void writeToAll(list_ptr descriptors, struct message *m);

struct message *readMessage(int readDescriptor);

struct message *readFromAll(list_ptr descriptors);

int isOperation(char c);

int getPriority(char c);

void performOne(list_ptr *vals, list_ptr *ops);

struct node *build(char *str);

#endif // UTILITY_H
