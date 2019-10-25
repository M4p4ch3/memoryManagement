
default: clean all reClean

all: a

a: mem.o main.o
	gcc mem.o main.o -o a.out

mem.o: mem.h
	gcc -c ./mem.c -I. -o mem.o

main.o: mem.h
	gcc -c ./main.c -I. -o main.o

clean:
	rm -f *.o

reClean:
	rm -f *.o