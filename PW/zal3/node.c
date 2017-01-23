#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "err.h"
#include "message.h"

void sendMessage(node_ptr node, message_ptr m) {
    writeMessage(node->mainWriteDescriptor, m);
}

node_ptr createEmptyNode() {
    node_ptr ret = (node_ptr)malloc(sizeof(node_t));
    ret->val = LINF;
    ret->type = -1;
    ret->inputDescriptors = createEmptyList();
    ret->outputDescriptors = createEmptyList();
    ret->operation = 0;
    ret->receivedVals = 0;
    ret->isProcessed = 0;
    int temp = 0;
    createPipe(&temp, &ret->mainWriteDescriptor);
    addInt(&ret->inputDescriptors, temp);
    allNodes[nodesCount++] = ret;
    return ret;
}

node_ptr createValueNode(long value) {
    node_ptr ret = createEmptyNode();
    ret->type = VALUE_NODE;
    ret->val = value;
    return ret;
}

node_ptr createBinaryOperationNode(char operation, node_ptr in1, node_ptr in2) {
    node_ptr ret = createEmptyNode();
    ret->type = BINARY_OPERATION_NODE;
    ret->operation = operation;
    ret->receivedVals = (message_ptr *)calloc(MAX_OPS, sizeof(message_ptr));
    ret->isProcessed = (int *)calloc(MAX_OPS, sizeof(int));
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
    ret->isProcessed = (int *)calloc(MAX_OPS, sizeof(int));
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

void dispatchInitialValues(int id, long *vals, int *isInCirciut) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (isInCirciut[i]) {
            if (vals[i] != LINF) {
                fprintf(stderr, "dispatching to %d value %ld\n", i, vals[i]);
                sendMessage(getOrCreateVariableNode(i),
                            createStartWithValMessage(id, vals[i]));
            } else {
                fprintf(stderr, "dispatching to %d undefined value\n", i);
                sendMessage(getOrCreateVariableNode(i), createStartMessage(id));
            }
        }
    }
    fprintf(stderr, "finished dispatching\n");
}

void dispatchConsts(int inits_count) {
    for (int i = 0; i < nodesCount; i++) {
        if (allNodes[i]->type == VALUE_NODE) {
            sendMessage(allNodes[i], createStartMessage(inits_count));
        }
    }
}

void killAllProcesses() {
    for (int i = 0; i < nodesCount; i++) {
        sendMessage(allNodes[i], createExitMessage());
    }
    for (int i = 0; i < nodesCount; i++) {
        wait(0);
    }
}

void valueNodeLoop(node_ptr node) {
    fprintf(stderr, "value node loop %ld\n", node->val);
    while (1) {
        message_ptr in = readFromAll(node->inputDescriptors);
        switch (in->type) {
        case EXIT_MESSAGE:
            deleteMessage(in);
            deleteNode(node);
            exit(0);
            break;
        case START_MESSAGE: {
            for (int i = 0; i < in->init_id; i++) {
                writeToAll(node->outputDescriptors,
                           createResultMessage(i, node->val));
            }
            break;
        }
        default:
            syserr("unsupported message %d", in->type);
        }
        deleteMessage(in);
    }
}

void binaryOperationNodeLoop(node_ptr node) {
    while (1) {
        fprintf(stderr, "binop waiting\n");
        message_ptr in = readFromAll(node->inputDescriptors);
        fprintf(stderr, "bin read:\n");
        printMessage(in);
        switch (in->type) {
        case EXIT_MESSAGE:
            deleteMessage(in);
            deleteNode(node);
            exit(0);
            break;
        case UNDEFINED_RESULT_MESSAGE: {
            if (node->isProcessed[in->init_id]) {
                break; // already processed this, ignoring
            }
            node->isProcessed[in->init_id] = 1;
            writeToAll(node->outputDescriptors,
                       createUndefinedResultMessage(in->init_id));
            deleteMessage(node->receivedVals[in->init_id]);
            node->receivedVals[in->init_id] = 0;
            deleteMessage(in);
            break;
        }
        case RESULT_MESSAGE: {
            if (node->isProcessed[in->init_id] != 0) {
                break; // already processed this, ignoring
            }
            fprintf(stderr, "end\n");
            if (node->receivedVals[in->init_id] != 0) {
                long a = in->value;
                long b = node->receivedVals[in->init_id]->value;
                long res = 0;
                if (node->operation == '+') {
                    res = a + b;
                } else if (node->operation == '*') {
                    res = a * b;
                } else {
                    syserr("operator %c not supported", node->operation);
                }
                writeToAll(node->outputDescriptors,
                           createResultMessage(in->init_id, res));
                node->isProcessed[in->init_id] = 1;
                deleteMessage(node->receivedVals[in->init_id]);
                node->receivedVals[in->init_id] = 0;
                deleteMessage(in);
            } else {
                node->receivedVals[in->init_id] = in;
            }
            break;
        }
        default:
            syserr("unsupported operation %d", in->type);
            deleteMessage(in);
            break;
        }
    }
}

void unaryOperationNodeLoop(node_ptr node) {
    while (1) {
        message_ptr in = readFromAll(node->inputDescriptors);
        switch (in->type) {
        case EXIT_MESSAGE:
            deleteNode(node);
            exit(0);
            break;
        case RESULT_MESSAGE: {
            message_ptr out = createResultMessage(in->init_id, -1 * in->value);
            writeToAll(node->outputDescriptors, out);
            break;
        }
        case UNDEFINED_RESULT_MESSAGE: {
            message_ptr out = createUndefinedResultMessage(in->init_id);
            writeToAll(node->outputDescriptors, out);
            break;
        }
        default:
            syserr("unsupported message %d", in->type);
        }
        deleteMessage(in);
    }
}

void variableNodeLoop(node_ptr node) {
    fprintf(stderr, "LOOP var %ld\n", node->val);
    while (1) {
        message_ptr in = readFromAll(node->inputDescriptors);
        fprintf(stderr, "var %ld read:\n", node->val);
        printMessage(in);
        switch (in->type) {
        case EXIT_MESSAGE:
            deleteNode(node);
            exit(0);
            break;
        case START_MESSAGE:
            if (node->isProcessed[in->init_id] != 0) {
                break; // this id was processed already, ignoring
            }
            if (getLen(node->inputDescriptors) == 1) { // only main input
                // we didn't get an initial value, so we should just send
                // undefined
                fprintf(stderr, "sending 0\n");
                writeToAll(node->outputDescriptors,
                           createUndefinedResultMessage(in->init_id));
                node->isProcessed[in->init_id] = 1;
            }
            break;
        case UNDEFINED_RESULT_MESSAGE: // fall through
        case RESULT_MESSAGE:           // fall through
        case START_WITH_VAL_MESSAGE: { // this message always comes before the
                                       // results from parent nodes
            if (node->isProcessed[in->init_id] != 0) {
                break; // this id was processed already, ignoring
            }
            message_ptr out = 0;
            if (in->type == UNDEFINED_RESULT_MESSAGE) {
                out = createUndefinedResultMessage(in->init_id);
            } else {
                out = createResultMessage(in->init_id, in->value);
            }
            writeToAll(node->outputDescriptors, out);
            node->isProcessed[in->init_id] = 1;
            break;
        }
        default:
            syserr("var %d received unsupported message %d", node->val,
                   in->type);
            break;
        }
        deleteMessage(in);
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
        syserr("unknown node type %d", node->type);
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
        break;
    }
}

void startProcessesForAllNodes() {
    fprintf(stderr, "starting %d processes\n", nodesCount);
    for (int i = 0; i < nodesCount; i++) {
        startProcess(allNodes[i]);
    }
}

void deleteNode(node_ptr n) {
    if (n == 0) {
        return;
    }
    deleteListOfInts(n->inputDescriptors);
    deleteListOfInts(n->outputDescriptors);
    free(n->isProcessed);
    if (n->receivedVals != 0) {
        for (int i = 0; i < MAX_OPS; i++) {
            deleteMessage(n->receivedVals[i]);
        }
        free(n->receivedVals);
    }
    free(n);
}
