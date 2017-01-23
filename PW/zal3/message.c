#include "message.h"
#include <stdio.h>

#include <stdlib.h>

message_ptr createMessage(int type, int init_id, int value) {
    message_ptr ret = (message_ptr)malloc(sizeof(message_t));
    ret->init_id = init_id;
    ret->value = value;
    ret->type = type;
    return ret;
}

message_ptr createStartMessage(int init_id) {
    message_ptr ret = createMessage(START_MESSAGE, init_id, 0); 
    return ret;
}

message_ptr createExitMessage() {
    message_ptr ret = createMessage(EXIT_MESSAGE, 0, 0); 
    return ret;
}

message_ptr createStartWithValMessage(int init_id, int value) {
    message_ptr ret = createMessage(START_WITH_VAL_MESSAGE, init_id, value); 
    return ret;
}

message_ptr createResultMessage(int init_id, int value) {
    message_ptr ret = createMessage(RESULT_MESSAGE, init_id, value);
    return ret;
}

message_ptr createUndefinedResultMessage(int init_id) {
    message_ptr ret = createMessage(UNDEFINED_RESULT_MESSAGE, init_id, 0);
    return ret;
}

int getType(message_ptr m) {
    return m->type;
}

int getValue(message_ptr m) {
    return m->value;
}

int getId(message_ptr m) {
    return m->init_id;
}

void printMessage(message_ptr m) {
    fprintf(stderr, "-------------message-----------");
    switch (m->type) {
        case START_MESSAGE:
            fprintf(stderr, "START\n");
            break;
        case START_WITH_VAL_MESSAGE:
            fprintf(stderr, "START with val \n");
            break;
        case EXIT_MESSAGE:
            fprintf(stderr, "EXIT message\n");
            break;
        case RESULT_MESSAGE:
            fprintf(stderr, "result message\n");
            break;
        case UNDEFINED_RESULT_MESSAGE:
            fprintf(stderr, "undefined result message\n");
            break;
    }
    fprintf(stderr, "id %d type %d val %ld\n", m->init_id, m->type, m->value);
    fprintf(stderr, "-------------message-----------\n\n");
}

void deleteMessage(message_ptr m) {
    free(m);
}
