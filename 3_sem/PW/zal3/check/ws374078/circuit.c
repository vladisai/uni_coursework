#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utility.h"
#include "node.h"

int main() {
    int n, k, v;
    scanf("%d%d%d\n", &n, &k, &v);

    for (int i = 1; i <= k; i++) {
        char currentExpression[EXPR_LEN];
        printf("%d ", i);
        int currentIndex, variable;
        scanf("%d x[%d] = ", &currentIndex, &variable);
        if (isInCircuit[variable] == 2) {
            printf("%d F\n", i);
            return 0;
        }
        isInCircuit[variable] = 2;
        fgets(currentExpression, EXPR_LEN, stdin);
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

    int correctInputs = 0;

    for (int i = k + 1; i <= n; i++) {
        char currentExpression[EXPR_LEN];
        int currentIndex;
        scanf("%d ", &currentIndex);
        fgets(currentExpression, EXPR_LEN, stdin);
        long *vals = parseValues(currentExpression);
        if (vals == 0) {
            printf("%d F\n", i);
            continue;
        }
        for (int i = 0; i < v; i++) {
        }

        if (!checkExistence(vals, v)) {
            printf("%d F\n", i);
        } else {
            correctInputs++;
            dispatchInitialValues(i - k - 1, vals, isInCircuit);
            dispatchConsts(i - k - 1);
        }
        free(vals);
    }

    for (int i = 0; i < correctInputs; i++) {
        message_ptr res = readMessage(readRoot);
        switch (res->type) {
            case RESULT_MESSAGE:
                printf("%d P %ld\n", res->init_id + k + 1, res->value);
                break;
            case UNDEFINED_RESULT_MESSAGE:
                printf("%d F\n", res->init_id + k + 1);
                break;
            default:
                break;
        }
        deleteMessage(res);
    }

    killAllProcesses();

    return 0;
}
