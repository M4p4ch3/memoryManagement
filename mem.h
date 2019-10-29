
# if (!(defined(MEM_H)))
# define MEM_H

# include <stdlib.h>
# include <stdio.h>

# define TOTAL_SIZE 1000
# define MIN_BLOC_DATA_SIZE 5
# define DISP_LINE_LEN 50

# define RES "\x1B[0m"
# define RED "\x1B[31m"
# define GRN "\x1B[32m"
# define YEL "\x1B[33m"
# define BLU "\x1B[34m"
# define MAG "\x1B[35m"
# define CYN "\x1B[36m"
# define WHT "\x1B[37m"

typedef int bool;
# define true 1
# define false 0

# define BLOC_HEAD(ptr) ((void *)((unsigned long)ptr - sizeof(block_t)))
# define BLOC_MEM(ptr) ((void *)((unsigned long)ptr + sizeof(block_t)))

typedef struct block_t
{
    bool alloc;
    size_t dataSize;
    struct block_t * prev;
    struct block_t * next;
} block_t;

void * myMalloc(int size);
void myFree(void * address);
void disp();
void init();
void defrag();

# endif // # if (!(defined(MEM_H)))