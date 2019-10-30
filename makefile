
default: clean all reClean

all: env.out test.out mem.o

env.out: env.o mem.o
	gcc env.o mem.o -o env.out

test.out: mem.o test.o
	gcc mem.o test.o -o test.out

env.o: mem.h
	gcc -c ./env.c -I. -o env.o

test.o: mem.h
	gcc -c ./test.c -I. -o test.o

mem.o: mem.h
	gcc -c ./mem.c -I. -o mem.o

clean:
	rm -f *.o

reClean:
	rm -f *.o