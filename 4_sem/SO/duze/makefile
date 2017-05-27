count = 1000000

a: a.c
	clang a.c -o a

clean:
	-rm zero; rm one

run_0: a clean
	time ./a zero 0 $(count) 

run_1: a clean
	time ./a one 1 $(count)
