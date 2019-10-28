
# include "mem.h"

bool bInit = false;
int freeSize = TOTAL_SIZE;
char memory[TOTAL_SIZE];
block_t * firstBloc = NULL;

void init()
{
    for (int i = 0; i < TOTAL_SIZE; i = i + 1)
    {
        memory[i] = '-';
    }
    
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

void insert(block_t * block, block_t * prev, block_t * next)
{
    block->prev = prev;
    block->next = next;
    if (prev != NULL)
    {
        prev->next = block;
    }
    if (next != NULL)
    {
        next->prev = block;
    }
}

void mergePrev(block_t * block)
{
    // If prev block_t is free
    if (block->prev != NULL && block->prev->alloc == false)
    {
        block->dataSize = block->prev->dataSize + block->dataSize + sizeof(block_t);
        if (block->prev->prev != NULL)
        {
            block->prev->prev->next = block;
        }
        block->prev = block->prev->prev;
        block = block->prev;
    }
}

void mergeNext(block_t * block)
{
    // If next block_t is free
    if (block->next != NULL && block->next->alloc == false)
    {
        block->dataSize = block->dataSize + block->next->dataSize + sizeof(block_t);
        if (block->next->next != NULL)
        {
            block->next->next->prev = block;
        }
        block->next = block->next->next;
    }
}

void defrag()
{
    block_t * block = firstBloc;
    
    while (block)
    {
        // Look for free Block followed by alloc Block
        while (block != NULL && block->next != NULL &&
            !(block->alloc == false && block->next->alloc == true))
        {
            block = block->next;
        }

        if (block->next)
        {
            // Copy Data to prev Free Block
            myMemCpy(BLOC_MEM(block->next), BLOC_MEM(block), block->next->dataSize);

            // Switch Blocks data
            unsigned int tempSize = block->next->dataSize;
            block->next->alloc = false;
            block->next->dataSize = block->dataSize;
            block->alloc = true;
            block->dataSize = tempSize;

            // Try to merge next free Block
            mergeNext(block->next);
        }

        block = block->next;
    }
}

void * myMalloc(int size)
{
    if (!bInit) init();

    // Not enough free Size
    if (freeSize < size + sizeof(block_t)) return NULL;

    // Get first suited free Block
    block_t * block = firstBloc;
    while (block != NULL && !(block->alloc == false && block->dataSize > size + sizeof(block_t)))
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
        while (block != NULL && !(block->alloc == false && block->dataSize > size + sizeof(block_t)))
        {
            block = block->next;
        }
        // No free Block found
        if (block == NULL) return NULL;
    }
    //*/

    // Still some place in Free Block
    if (block->dataSize > size)
    {
        // Create new free Block
        block_t * newFreeBloc = BLOC_MEM(block) + size;
        newFreeBloc->alloc = false;
        newFreeBloc->dataSize = block->dataSize - (size + sizeof(block_t));

        insert(newFreeBloc, block, block->next);

        block->dataSize = size;
    }

    // Alloc Block
    block->alloc = true;
    freeSize = freeSize - (size + sizeof(block_t));

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
    freeSize = freeSize + block->dataSize + sizeof(block_t);
    
    // Try to merge aside free Blocks
    mergePrev(block);
    mergeNext(block);
}

void write(void * ptr, char * str, int len)
{
    // Find alloc Block
    block_t * block = getBloc(BLOC_HEAD(ptr));
    // Alloc Block not found
    if (block == NULL) return;
    // block_t is not allocated
    if (block->alloc == false) return;
    // To long Message
    if (len > block->dataSize) return;

    for (int i = 0; i < len; i = i + 1)
    {
        *((char *)(ptr + i * sizeof(char))) = str[i];
    }
}

void disp()
{
    int iBloc = 0;
    int line = 0;
    int row = 0;
    char cha;
    block_t * block = firstBloc;
    printf("\r\n");
    while (block != NULL)
    {
        printf(" ====================\r\n");
        printf(" |     Block %3d    |\r\n", iBloc);
        printf(" |  %p  |\r\n", block);
        printf(" | size : %3d (%3d) |\r\n", block->dataSize, (int) (block->dataSize + sizeof(block_t)));
        if (block->alloc)
        {
            printf(" |     Allocated    |\r\n");
        }
        else
        {
            printf(" |       Free       |\r\n");
        }
        printf(" |------------------|\r\n");
        while (line < block->dataSize / 16 + 1)
        {
            printf(" | ");
            row = 0;
            while (line * 16 + row < block->dataSize && row < 16)
            {
                cha = *((char *)(BLOC_MEM(block) + (line * 16 + row) * sizeof(char)));
                if (cha >= 32 && cha <= 127) printf("%c", cha);
                else printf(" ");
                row = row + 1;
            }
            while (row < 16)
            {
                printf(" ");
                row = row + 1;
            }
            printf(" |\r\n");
            line = line + 1;
        }
        printf(" ====================\r\n");
        if (block->next != NULL)
        {
            printf("     |          A\r\n");
            printf("     V          |\r\n");
        }
        block = block->next;
        iBloc = iBloc + 1;
    }
    printf("\r\n");
}
