all: node list utility err message
	gcc -Wall err.o message.o list.o utility.o node.o main.c -o main

run:
	./main

list: list.h list.c
	gcc -c list.c

utility: utility.h utility.c
	gcc -c utility.c

node: list node.h node.c
	gcc -c node.c

err: err.h err.c
	gcc -c err.c

message: message.h message.c
	gcc -c message.c
