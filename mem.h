
# if (!(defined(MEM_H)))
# define MEM_H

# include <stdlib.h>
# include <stdio.h>

# define TOTAL_SIZE 100

typedef int bool;
# define true 1
# define false 0

typedef struct Bloc
{
    bool alloc;
    int address;
    int size;
    struct Bloc * prev;
    struct Bloc * next;
} Bloc;

// Private
void init();
void defrag();

// Public
int myMalloc(int size);
void myFree(int address);
void write(int address, int len, char * str);
int read(int address, int offset);
void dispData();
void dispBlocs();

# endif // # if (!(defined(MEM_H)))