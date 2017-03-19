#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#include "node.h"

void addElement(list_ptr *l, void *p) {
    list_t *newList = (list_t *)malloc(sizeof(list_t));
    newList->tail = *l;
    newList->element = p;
    *l = newList;
}

void addInt(list_ptr *l, int e) {
    int *ptr = (int*)malloc(sizeof(int));
    *ptr = e;
    addElement(l, ptr);
}

void addNode(list_ptr *l, node_ptr e) {
    node_ptr *ptr = (node_ptr*)malloc(sizeof(node_ptr));
    *ptr = e;
    addElement(l, ptr);
}

int getTopInt(list_ptr l) { return *((int*)l->element); }

node_ptr getTopNode(list_ptr l) { return *((node_ptr*)l->element); }

list_ptr popInt(list_ptr l) {
    list_t *ret = l->tail;
    free(l->element);
    free(l);
    return ret;
}

list_ptr popNode(list_ptr l) {
    list_t *ret = l->tail;
    free(l);
    return ret;
}

int popAndGetInt(list_ptr *l) {
    int val = getTopInt(*l);
    *l = popInt(*l);
    return val;
}

node_ptr popAndGetNode(list_ptr *l) {
    node_ptr val = getTopNode(*l);
    *l = popNode(*l);
    return val;
}

void printAllInts(list_ptr l) {
    list_ptr p = l;
    printf("[");
    while (p != 0) {
        printf("%d, ", getTopInt(p));
        p = p->tail;
    }
    printf("]");
}

void shift(list_ptr *p) {
    if (*p != 0)
        *p = (*p)->tail;
}

int isEmpty(list_ptr p) {
    return p == 0;
}

int getLen(list_ptr p) {
    int res = 0;
    while (p != 0) {
        res++;
        shift(&p);
    }
    return res;
}

list_ptr createEmptyList() {
    return 0;
}

void deleteListOfInts(list_ptr p) {
    while (p != 0) {
        free(p->element);
        list_ptr nxt = p->tail;
        free(p);
        p = nxt;
    }
}

void deleteListOfNodes(list_ptr p) {
    while (p != 0) {
        deleteNode((node_ptr)p->element);
        list_ptr nxt = p->tail;
        free(p);
        p = nxt;
    }
}
