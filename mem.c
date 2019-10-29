
# include "mem.h"

bool bInit = false;
int freeSize = TOTAL_SIZE - sizeof(block_t);
char memory[TOTAL_SIZE];
char tempMemory[TOTAL_SIZE / 10];
block_t * firstBloc = NULL;

void init()
{
    firstBloc = (block_t *) &memory[0];
    firstBloc->alloc = false;
    firstBloc->dataSize = TOTAL_SIZE - sizeof(block_t);
    firstBloc->prev = NULL;
    firstBloc->next = NULL;
    bInit = true;
}

void myMemCpy(void * src, void * dest, int len)
{
    for (int i = 0; i < len; i = i + 1)
    {
        *((char *)(dest + i)) = *((char *)(src + i));
    }
}

block_t * getBloc(void * ptr)
{
    block_t * block = firstBloc;
    while (block != NULL && !(block == ptr))
    {
        block = block->next;
    }
    return block;
}

void mergeNext(block_t * block)
{
    // Null Block
    if (block == NULL) return;
    // Block is not free
    if (block->alloc != false) return;

    // If next Block is free
    if (block->next != NULL && block->next->alloc == false)
    {
        block->dataSize = block->dataSize + block->next->dataSize + sizeof(block_t);
        if (block->next->next != NULL)
        {
            block->next->next->prev = block;
        }
        block->next = block->next->next;
        // Update global free Size
        freeSize = freeSize + sizeof(block_t);
    }
}

void split(block_t * block, size_t size)
{
    block_t * newFreeBlock = BLOC_MEM(block) + size;
    newFreeBlock->alloc = false;
    newFreeBlock->dataSize = block->dataSize - size - sizeof(block_t);
    newFreeBlock->prev = block;
    newFreeBlock->next = block->next;

    block->dataSize = size;
    if (block->next != NULL)
    {
        block->next->prev = newFreeBlock;
    }
    block->next = newFreeBlock;

    freeSize = freeSize + newFreeBlock->dataSize;
}

void defrag()
{
    size_t dataSize;
    block_t * block = firstBloc;
    block_t * newFreeBloc;
    
    while (block != NULL)
    {
        // Look for free Block followed by alloc Block
        while (block != NULL && block->next != NULL &&
            !(block->alloc == false && block->next->alloc == true))
        {
            block = block->next;
        }

        if (block->next != NULL)
        {
            // Update global free Size
            freeSize = freeSize - block->dataSize;
            // Data Size of next alloc Block
            dataSize = block->next->dataSize;
            // Copy Data from next alloc Block to temp Memory
            myMemCpy(BLOC_MEM(block->next), tempMemory, dataSize);

            // Merge free Block and next alloc Block
            block->alloc = true;
            block->dataSize = block->dataSize + sizeof(block_t) + block->next->dataSize;
            block->next->next->prev = block;
            block->next = block->next->next;

            // Split merged Block
            split(block, dataSize);

            // Copy Data from temp Memory to alloc Block
            myMemCpy(tempMemory, BLOC_MEM(block), block->dataSize);

            // Try to merge new free Block with next Block
            if (block->next != NULL && block->next->next != NULL && block->next->next->alloc == false)
            {
                mergeNext(block->next);
            }
        }

        block = block->next;
    }
}

void * myMalloc(int size)
{
    if (!bInit) init();

    // Not enough free Size
    if (freeSize < size) return NULL;

    // Get first suited free Block
    block_t * block = firstBloc;
    while (block != NULL && !(block->alloc == false && block->dataSize >= size))
    {
        block = block->next;
    }
    // No free Block found
    if (block == NULL) // return NULL;
    //*
    {
        // Failed to find suited free Block
        // But enough free Size
        // Defragment Memory
        defrag();

        // 2nd Try
        block = firstBloc;
        while (block != NULL && !(block->alloc == false && block->dataSize >= size))
        {
            block = block->next;
        }
        // No free Block found
        if (block == NULL) return NULL;
    }
    //*/

    // Alloc Block
    block->alloc = true;
    // Update global free Size
    freeSize = freeSize - block->dataSize;

    // Enough Place for a new minimal Free Block
    if (block->dataSize >= size + sizeof(block_t) + MIN_BLOC_DATA_SIZE)
    {
        // Split Block
        split(block, size);
    }

    return BLOC_MEM(block);
}

void myFree(void * ptr)
{
    // Find alloc Block
    block_t * block = getBloc(BLOC_HEAD(ptr));
    // Alloc Block not found
    if (block == NULL) return;
    // block_t is not allocated
    if (block->alloc == false) return;

    // Free the Block
    block->alloc = false;
    // Update global free Size
    freeSize = freeSize + block->dataSize;
    
    // Try to merge aside free Blocks
    if (block->next != NULL && block->next->alloc == false)
    {
        mergeNext(block);
    }
    if (block->prev != NULL && block->prev->alloc == false)
    {
        mergeNext(block->prev);
    }
}

void disp()
{
    int i = 0;
    int iBloc = 0;
    int line = 0;
    int row = 0;
    int freeSizeSum = 0;
    char cha = ' ';
    block_t * block;
    
    printf("\r\n");
    block = firstBloc;
    while (block != NULL)
    {
        printf(" ==");
        for (i = 0; i < DISP_LINE_LEN; i = i + 1) printf("=");
        printf("== %p\r\n", block);

        printf(" | ");
        for (i = 0; i < (DISP_LINE_LEN - sizeof("Block 000")) / 2; i = i + 1) printf(" ");
        printf("Block %3d", iBloc);
        for (i = 0; i < (DISP_LINE_LEN - sizeof("Block 000")) / 2; i = i + 1) printf(" ");
        printf("  |\r\n");

        printf(" |  ");
        for (int i = 0; i < (DISP_LINE_LEN - sizeof("size : 000 (000)")) / 2; i = i + 1) printf(" ");
        printf("size : %3d (%3d)", (int) block->dataSize, (int) (block->dataSize + sizeof(block_t)));
        for (int i = 0; i < (DISP_LINE_LEN - sizeof("size : 000 (000)")) / 2; i = i + 1) printf(" ");
        printf("  |\r\n");

        printf(" |  ");
        if (block->alloc)
        {
            for (int i = 0; i < (DISP_LINE_LEN - sizeof("Allocated")) / 2; i = i + 1) printf(" ");
            printf("%sAllocated%s", RED, RES);
            for (int i = 0; i < (DISP_LINE_LEN - sizeof("Allocated")) / 2; i = i + 1) printf(" ");
        }
        else
        {
            for (int i = 0; i < (DISP_LINE_LEN - sizeof("Free")) / 2; i = i + 1) printf(" ");
            printf("%sFree %s", GRN, RES);
            for (int i = 0; i < (DISP_LINE_LEN - sizeof("Free")) / 2; i = i + 1) printf(" ");
            freeSizeSum = freeSizeSum + block->dataSize;
        }
        printf(" |\r\n");

        printf(" |-");
        for (int i = 0; i < DISP_LINE_LEN; i = i + 1) printf("-");
        printf("-| %p\r\n", BLOC_MEM(block));

        line = 0;
        row = 0;
        while (line * DISP_LINE_LEN + row < block->dataSize)
        {
            printf(" |");
            if (line == 0) printf("[");
            else printf(" ");
            row = 0;
            if (block->alloc == true) printf("%s", RED);
            else printf("%s", GRN);
            while (line * DISP_LINE_LEN + row < block->dataSize && row < DISP_LINE_LEN)
            {
                cha = *((char *)(BLOC_MEM(block) + (line * DISP_LINE_LEN + row) * sizeof(char)));
                if (cha >= 32 && cha <= 126) printf("%c", cha);
                else printf("-");
                row = row + 1;
            }
            printf("%s", RES);
            if (line * DISP_LINE_LEN + row >= block->dataSize)
            {
                printf("]");
                row = row + 1;
            }
            while (row < DISP_LINE_LEN + 1)
            {
                printf(" ");
                row = row + 1;
            }
            printf("|\r\n");
            line = line + 1;
            row = 0;
        }

        printf(" ==");
        for (int i = 0; i < DISP_LINE_LEN; i = i + 1) printf("=");
        printf("== %p\r\n", (void *)((unsigned long) BLOC_MEM(block) + block->dataSize));

        if (block->next != NULL)
        {
            printf("   ");
            for (int i = 0; i < DISP_LINE_LEN / 4; i = i + 1) printf(" ");
            printf("|");
            for (int i = 0; i < DISP_LINE_LEN / 2; i = i + 1) printf(" ");
            printf("A\r\n");
            printf("   ");
            for (int i = 0; i < DISP_LINE_LEN / 4; i = i + 1) printf(" ");
            printf("V");
            for (int i = 0; i < DISP_LINE_LEN / 2; i = i + 1) printf(" ");
            printf("|\r\n");
        }

        block = block->next;
        iBloc = iBloc + 1;
    }
    printf("\r\n");
    printf(" Free Size : %d\r\n", freeSize);
    printf(" Sum of Free Blocks Sizes : %d\r\n", freeSizeSum);
    printf("\r\n");
}
