vector: vector.cc vector.h point
	g++ -O2 -Wall vector.cc -c

point: point.cc point.h
	g++ -O2 -Wall point.cc -c

all: point
	g++ -O2 -Wall point.o vector.o main.cc -o main
