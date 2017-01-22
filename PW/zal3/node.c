#include "node.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "err.h"

void sendMessage(node_ptr node, int value) {
    writeInt(node->mainWriteDescriptor, value);
}

node_ptr createEmptyNode() {
    node_ptr ret = (node_ptr)malloc(sizeof(node_t));
    ret->val = INF;
    ret->inputDescriptors = createEmptyList();
    ret->outputDescriptors = createEmptyList();
    ret->operation = 0;
    createPipe(&ret->mainReadDescriptor, &ret->mainWriteDescriptor);
    allNodes[nodesCount++] = ret;
    return ret;
}

node_ptr createValueNode(int value) {
    node_ptr ret = createEmptyNode();
    ret->type = VALUE_NODE;
    ret->val = value;
    return ret;
}

node_ptr createBinaryOperationNode(char operation, node_ptr in1, node_ptr in2) {
    node_ptr ret = createEmptyNode();
    ret->type = BINARY_OPERATION_NODE;
    ret->operation = operation;
    tieNodes(in1, ret);
    tieNodes(in2, ret);
    return ret;
}

node_ptr createUnaryOperationNode(char operation, node_ptr in) {
    node_ptr ret = createEmptyNode();
    ret->type = UNARY_OPERATION_NODE;
    ret->operation = operation;
    tieNodes(in, ret);
    return ret;
}

node_ptr createVariableNode(int id) {
    node_ptr ret = createEmptyNode();
    ret->type = VARIABLE_NODE;
    ret->operation = 'x';
    ret->val = id;
    return ret;
}

node_ptr getOrCreateVariableNode(int id) {
    if (variables[id] == 0) {
        variables[id] = createVariableNode(id);
    }
    return variables[id];
}

void tieNodes(node_ptr sender,
              node_ptr receiver) { // creates a pipe between the two
    int read, write;
    createPipe(&read, &write);
    addInt(&sender->outputDescriptors, write);
    addInt(&receiver->inputDescriptors, read);
}

void dispatchInitialValues(int *vals) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (vals[i] != INF) {
            fprintf(stderr, "dispatching to %d value %d\n", i, vals[i]);
            sendMessage(getOrCreateVariableNode(i), vals[i]);
        }
    }
    fprintf(stderr, "finished dispatching\n");
}

void startAll() {
    for (int i = 0; i < nodesCount; i++) {
        sendMessage(allNodes[i], START_CMD);
    }
}

void killAllProcesses() {
    for (int i = 0; i < nodesCount; i++) {
        sendMessage(allNodes[i], EXIT_CMD);
        wait(0);
    }
}

void sendToAll(list_ptr descriptors, int value) {
    while (descriptors != 0) {
        writeInt(getTopInt(descriptors), value);
        descriptors = descriptors->tail;
    }
}

void valueNodeLoop(node_ptr node) {
    int val;
    while (1) {
        readInt(node->mainReadDescriptor, &val);
        if (val == EXIT_CMD) {
            exit(0); // close pipes?
        } else {
            sendToAll(node->outputDescriptors, val);
        }
    }
}

void binaryOperationNodeLoop(node_ptr node) {
    int val;
    while (1) {
        readInt(node->mainReadDescriptor, &val);
        if (val == EXIT_CMD) {
            exit(0); // close pipes?
        } else {
            int a, b;
            list_ptr input = node->inputDescriptors;
            readInt(getTopInt(input), &a);
            shift(&input);
            readInt(getTopInt(input), &b);

            int res = 0;
            if (a == UNDEFINED || b == UNDEFINED) {
                res = UNDEFINED;
            } else if (node->operation == '*') {
                res = a * b;
            } else if (node->operation == '+') {
                res = a + b;
            }

            sendToAll(node->outputDescriptors, res);
        }
    }
}

void unaryOperationNodeLoop(node_ptr node) {
    int val;
    while (0) {
        readInt(node->mainReadDescriptor, &val);
        if (val == EXIT_CMD) {
            exit(0); // close pipes?
        } else {
            int a;
            int desc = getTopInt(node->inputDescriptors);
            readInt(desc, &a);

            int res = 0;
            if (a == UNDEFINED) {
                res = UNDEFINED;
            } else if (node->operation == '-') {
                res = -a;
            } 
            sendToAll(node->outputDescriptors, res);
        }
    }
}

void variableNodeLoop(node_ptr node) {
    int val;
    fprintf(stderr, "LOOP var %d\n", node->val);
    while (0) {
        readInt(node->mainReadDescriptor, &val);
        fprintf(stderr, "I just read %d\n", val);
        if (val == EXIT_CMD) {
            exit(0); // close pipes?
        } else {
            if (val != START_CMD) {
                int tmp;
                readInt(node->mainReadDescriptor, &tmp); // don't need that
            }
            int a;
            int desc = getTopInt(node->inputDescriptors);
            readInt(desc, &a);

            int res = 0;
            if (a == UNDEFINED && val == START_CMD) {
                res = UNDEFINED;
            } else if (a == UNDEFINED && val != START_CMD) {
                res = val;
            } else if (a != UNDEFINED && val == START_CMD) {
                res = a;
            }
            sendToAll(node->outputDescriptors, res);
        }
    }
}

void nodeLoop(node_ptr node) {
    switch (node->type) {
    case VALUE_NODE:
        valueNodeLoop(node);
        break;
    case BINARY_OPERATION_NODE:
        binaryOperationNodeLoop(node);
        break;
    case UNARY_OPERATION_NODE:
        unaryOperationNodeLoop(node);
        break;
    case VARIABLE_NODE:
        variableNodeLoop(node);
        break;
    default:
        // error, unknown node type
        break;
    }
}

void startProcess(node_ptr node) {
    switch (fork()) {
    case -1:
        // error message
        break;
    case 0:
        if (close(node->mainWriteDescriptor) == -1) {
            syserr("close mainWriteDescriptor");
        }
        nodeLoop(node);
        exit(0);
        break;
    default:
        if (close(node->mainReadDescriptor) == -1) {
            syserr("close mainWriteDescriptor");
        }
        break;
    }
}

void startProcessesForAllNodes() {
    for (int i = 0; i < nodesCount; i++) {
        startProcess(allNodes[i]);
    }
}
