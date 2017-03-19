all: clear strdeque strdequeconst main
	g++ -std=c++14 -Wall -O2 main.o strdeque.o strdequeconst.o -o main

ctest: clear strdeque strdequeconst main
	cc -Wall -O2 strdeque.o strdequeconst.o -lstdc++ strdeque_test1.c -o strdeque_test1 & ./strdeque_test1

testa: clear strdeque strdequeconst main
	g++ -Wall -O2 strdeque.o strdequeconst.o strdeque_test2a.cc -o strdeque_test2a

testb: clear strdeque strdequeconst main
	g++ -Wall -O2 strdeque.o strdequeconst.o strdeque_test2b.cc -o strdeque_test2b

call: clear strdeque strdequeconst cmain 
	cc -Wall -O2 cmain.o strdeque.o strdequeconst.o -lstdc++ -o cmain

cmain: cmain.c
	cc -Wall -O2 -c cmain.c

strdeque: strdeque.cc strdeque.h
	g++ -Wall -O2 -std=c++14 -c strdeque.cc

strdequeconst: strdequeconst.cc strdequeconst.h
	g++ -Wall -O2 -std=c++14 -c strdequeconst.cc

main: main.cc
	g++ -Wall -O2 -std=c++14  -c main.cc

clear:
	rm main.o strdeque.o strdequeconst.o main | true 
