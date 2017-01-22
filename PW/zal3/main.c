#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utility.h"
#include "node.h"

int n, k, v;

// reverse polish notation

int isOperation(char c) {
    if (c == '*')
        return 1;
    else if (c == '+')
        return 1;
    else if (c == '-')
        return 1;
    else if (c == 'x')
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
    else if (c == 'x')
        return 4;
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
                ops = pop(ops);
            } else if (isdigit(str[i])) {
                int x = parseNumber(str, &i);
                addNode(&vals, createValueNode(x));
            } else if (str[i] == 'x') {
                i += 2;
                int x = parseNumber(str, &i);
                i++;
                addNode(&vals, createVariableNode(x));
            }
        }
    }
    while (ops != 0) {
        performOne(&vals, &ops);
    }
    return popAndGetNode(&vals);
}

int main() {
    freopen("input.txt", "r", stdin);
    /*freopen("/dev/null", "w", stderr);*/

    scanf("%d%d%d", &n, &k, &v);

    for (int i = 1; i <= k; i++) {
        char currentExpression[EXPR_LEN];
        printf("%d ", i);
        int currentIndex, variable;
        scanf("%d x[%d] = ", &currentIndex, &variable);
        /*if (isInCircuit[variable]) {*/
        /*    printf("F\n");*/
        /*    exit(0);*/
        /*}*/
        isInCircuit[variable] = 1;
        fgets(currentExpression, EXPR_LEN, stdin);
        /*printf("\nFound expression number %d for %d which is:\n%s \n",*/
        /*       currentIndex, variable, currentExpression);*/
        addEdges(variable, currentExpression);
        if (checkCycles()) {
            printf("F");
            return 0;
        } else {
            printf("P");
            node_ptr res = build(currentExpression);
            node_ptr var = getOrCreateVariableNode(variable);
            tieNodes(res, var);
        }
        printf("\n");
    }

    isInCircuit[0] = 1; // 0 is always in the circuit, even if it wasn't
                        // mentioned in the input

    for (int i = 0; i < v; i++) {
        if (isInCircuit[i]) {
            getOrCreateVariableNode(i);
        }
    }

    int readRoot, writeRoot;
    createPipe(&readRoot, &writeRoot);
    addInt(&getOrCreateVariableNode(0)->outputDescriptors, writeRoot);
    // close write here

    fflush(stdout);
    startProcessesForAllNodes();

    for (int i = k + 1; i <= n; i++) {
        char currentExpression[EXPR_LEN];
        printf("%d ", i);
        int currentIndex;
        scanf("%d ", &currentIndex);
        fgets(currentExpression, EXPR_LEN, stdin);
        int *vals = parseValues(currentExpression);

        for (int i = 0; i < v; i++) {
            fprintf(stderr, "%d=%d ", i, vals[i] == INF ? -1 : vals[i]);
        }
        fprintf(stderr, "\n");

        if (!checkExistence(vals, v)) {
            printf("F");
            fprintf(stderr, "Check existence\n");
        } else if (!checkReachability(vals, 0)) {
            printf("F");
            fprintf(stderr, "check reachability\n");
        } else {
            printf("P ");
            fprintf(stderr, "dispatching\n");
            dispatchInitialValues(vals);
            fprintf(stderr, "starting\n");
            startAll();
            fprintf(stderr, "Supposed to be computing right now\n");
            int val;
            readInt(readRoot, &val);
            printf("%d", val);
        }
        free(vals);
        printf("\n");
    }

    killAllProcesses();

    return 0;
}
