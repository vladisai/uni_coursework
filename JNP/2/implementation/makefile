all: clear strdeque main
	g++ main.o strdeque.o -o main

call: clear strdeque cmain
	cc cmain.o strdeque.o -lstdc++ -o cmain

cmain: cmain.c
	gcc -c cmain.c

strdeque: strdeque.cc strdeque.h
	g++ -c strdeque.cc

main: main.cc
	g++ -c main.cc

clear:
	rm main.o strdeque.o main | true 
