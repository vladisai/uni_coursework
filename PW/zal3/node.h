#ifndef NODE_H 
#define NODE_H

#include "utility.h"
#include "list.h"

#define START_CMD INF
#define EXIT_CMD -INF

#define UNDEFINED (-INF + 1)

#define VALUE_NODE 0
#define BINARY_OPERATION_NODE 1
#define UNARY_OPERATION_NODE 2
#define VARIABLE_NODE 3

typedef struct node {
    int mainReadDescriptor;
    int mainWriteDescriptor;
    list_ptr inputDescriptors;
    list_ptr outputDescriptors;
    char operation;
    int val;
    int type;
} node_t, *node_ptr;

node_ptr variables[MAX_VARS];
node_ptr allNodes[MAX_NODES];
int nodesCount;

void createPipe(int *read, int *write);

node_ptr createValueNode(int value);

node_ptr createBinaryOperationNode(char operation, node_ptr in1, node_ptr in2);

node_ptr createUnaryOperationNode(char operation, node_ptr in);

node_ptr createVariableNode(int id);

node_ptr getOrCreateVariableNode(int id);

void tieNodes(node_ptr sender, node_ptr receiver); // creates a pipe between the two

void dispatchInitialValues(int *vals);

void startAll();

void killAllProcesses();

void startProcessesForAllNodes();

int getLastId();

#endif // NODE_H
