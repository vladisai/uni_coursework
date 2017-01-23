#ifndef NODE_H 
#define NODE_H

#include "utility.h"
#include "list.h"
#include "message.h"

#define VALUE_NODE 4
#define BINARY_OPERATION_NODE 1
#define UNARY_OPERATION_NODE 2
#define VARIABLE_NODE 3

struct message;

typedef struct node {
    int mainWriteDescriptor;
    list_ptr inputDescriptors;
    list_ptr outputDescriptors;
    char operation;
    long val;
    int type;
    struct message **receivedVals;
    int *isProcessed;
} node_t, *node_ptr;

node_ptr variables[MAX_VARS];
node_ptr allNodes[MAX_NODES];
int nodesCount;

void createPipe(int *read, int *write);

node_ptr createValueNode(long value);

node_ptr createBinaryOperationNode(char operation, node_ptr in1, node_ptr in2);

node_ptr createUnaryOperationNode(char operation, node_ptr in);

node_ptr getOrCreateVariableNode(int id);

void tieNodes(node_ptr sender, node_ptr receiver); // creates a pipe between the two

void dispatchInitialValues(int id, long *vals, int *isInCirciut);

void dispatchConsts(int inits_count);

void startAll();

void killAllProcesses();

void startProcessesForAllNodes();

int getLastId();

void deleteNode(node_ptr n);

#endif // NODE_H
