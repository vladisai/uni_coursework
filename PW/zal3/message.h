#ifndef MESSAGE_H
#define MESSAGE_H

#define START_MESSAGE 1
#define EXIT_MESSAGE -1
#define START_WITH_VAL_MESSAGE 2
#define RESULT_MESSAGE 3
#define UNDEFINED_RESULT_MESSAGE 4

typedef struct message {
    int type;
    long value;
    int isValueUndefined;
    int init_id; // id of the initialization
} message_t, *message_ptr;

message_ptr createMessage(int type, int init_id, int value);

message_ptr createStartMessage(int init_id);

message_ptr createExitMessage();

message_ptr createStartWithValMessage(int init_id, int value);

message_ptr createResultMessage(int init_id, int value);

message_ptr createUndefinedResultMessage(int init_id);

int getType(message_ptr m);

int getValue(message_ptr m);

int getId(message_ptr m);

void printMessage(message_ptr m);

void deleteMessage(message_ptr m);

#endif // MESSAGE_H
