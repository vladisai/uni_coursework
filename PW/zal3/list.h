#ifndef LIST_H
#define LIST_H

struct node;

// list, which is more of a stack here
typedef struct list {
    void *element;
    struct list *tail;
} list_t, *list_ptr;


void addInt(list_ptr *l, int e);

void addNode(list_ptr *l, struct node *e);

int getTopInt(list_ptr l);

struct node *getTopNode(list_ptr l);

list_ptr pop(list_ptr l);

int popAndGetInt(list_ptr *l);

struct node *popAndGetNode(list_ptr *l);

void printAll(list_ptr l);

void shift(list_ptr *p);

int isEmpty(list_ptr p);

list_ptr createEmptyList();

#endif // LIST_H
