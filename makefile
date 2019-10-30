
default: clean all reClean

all: mem.o env.out

env.out: mem.o env.o
	gcc mem.o env.o -o env.out

mem.o: mem.h
	gcc -c ./mem.c -I. -o mem.o

env.o: mem.h
	gcc -c ./env.c -I. -o env.o

clean:
	rm -f *.o

reClean:
	rm -f *.o