
# if (!(defined(MEM_H)))
# define MEM_H

# include <stdlib.h>
# include <stdio.h>

# define TOTAL_SIZE 1000

typedef int bool;
# define true 1
# define false 0

# define BLOC_HEAD(ptr) ((void *)((unsigned long)ptr - sizeof(block_t)))
# define BLOC_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(block_t)))

typedef struct block_t
{
    bool alloc;
    unsigned int dataSize;
    struct block_t * prev;
    struct block_t * next;
} block_t;

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