
# if (!(defined(MEM_H)))
# define MEM_H

# include <stdlib.h>
# include <stdio.h>

# define TOTAL_SIZE 1000

typedef int bool;
# define true 1
# define false 0

typedef struct Bloc
{
    bool alloc;
    int size;
    struct Bloc * prev;
    struct Bloc * next;
} Bloc;

// Private
void init();
void defrag();

// Public
void * myMalloc(int size);
void myFree(void * address);
void write(void * address, char * str, int len);
void myMemCpy(void * src, void * dest, int len);
void disp();

# endif // # if (!(defined(MEM_H)))