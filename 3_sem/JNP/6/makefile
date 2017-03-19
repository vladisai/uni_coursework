all: helper.o citizen.o monster.o smalltown.o
	g++ helper.o citizen.o monster.o smalltown.o horror_example.cc -Wall -std=c++14 -o horror_example

citizen.o: helper.o citizen.h citizen.cc 
	g++ -c citizen.cc

helper.o: helper.h helper.cc
	g++ -c helper.cc

monster.o: helper.o monster.h monster.cc
	g++ -c monster.cc

smalltown.o: smalltown.cc smalltown.h
	g++ -c smalltown.cc
