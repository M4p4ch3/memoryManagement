
# if (!(defined(MEM_H)))
# define MEM_H

# include <stdlib.h>
# include <stdio.h>

# define TOTAL_SIZE 1000
# define MIN_BLOCK_DATA_SIZE 5
# define DISP_LINE_LEN 50

# define COLOR_RES "\x1B[0m"
# define COLOR_RED "\x1B[31m"
# define COLOR_GRN "\x1B[32m"

typedef int bool;
# define true 1
# define false 0

# define BLOCK_HEAD(ptr) ((void *)((unsigned long)ptr - sizeof(block_t)))
# define BLOCK_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(block_t)))

// Memory Block
typedef struct block_t
{
    // Unique ID
    unsigned int id;
    // Allocation State
    bool alloc;
    // Data Size
    size_t dataSize;
    // Prev Block
    struct block_t * prev;
    // Next Block
    struct block_t * next;
} block_t;

void * myMalloc(int size);
void myFree(void * address);
void * getPtr(int id);
void disp();
void prettyDisp();

# endif // # if (!(defined(MEM_H)))