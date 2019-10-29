
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
        freeSize = freeSize + sizeof(block_t);
    }
}

void defrag()
{
    // size_t dataSize;
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
            // block is free
            // block->next is alloc
            // "Switch" block and block->next
            // "Switch" free and alloc Block

            // Copy Data from next alloc Block to temp Memory
            myMemCpy(BLOC_MEM(block->next), tempMemory, block->next->dataSize);
            // dataSize = block->next->dataSize;

            // Alloc free Block
            block->alloc = true;

            // Create new free Block
            // Address : block Memory + dataSize of next alloc Bloc
            newFreeBloc = BLOC_MEM(block) + block->next->dataSize;
            newFreeBloc->alloc = false;
            // Size of the former free Block
            newFreeBloc->dataSize = block->dataSize;

            // Update Size to match next alloc Block Size
            block->dataSize = block->next->dataSize;

            // Update links
            newFreeBloc->next = block->next->next;
            block->next->next->prev = newFreeBloc;
            newFreeBloc->prev = block;
            block->next = newFreeBloc;

            // Copy Data from temp Memory to alloc Block
            myMemCpy(tempMemory, BLOC_MEM(block), block->dataSize);

            // Try to merge new free Block with next Block
            if (newFreeBloc->next != NULL && newFreeBloc->next->alloc == false)
            {
                mergeNext(newFreeBloc);
            }
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

void write(void * ptr, char * str, int len)
{
    // Null Pointer
    if (ptr == NULL) return;
    // No data
    if (str == NULL || len <= 0) return;

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
    int freeSizeSum = 0;
    char cha = ' ';
    block_t * block;
    
    printf("\r\n");
    block = firstBloc;
    while (block != NULL)
    {
        printf(" ======================== %p\r\n", block);
        printf(" |       Block %3d      |\r\n", iBloc);
        printf(" |   size : %3d (%3d)   |\r\n", block->dataSize, (int) (block->dataSize + sizeof(block_t)));
        if (block->alloc)
        {
            printf(" |       %sAllocated%s      |\r\n", RED, RES);
        }
        else
        {
            printf(" |         %sFree%s         |\r\n", GRN, RES);
            freeSizeSum = freeSizeSum + block->dataSize;
        }
        printf(" |----------------------| %p\r\n", BLOC_MEM(block));
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
        printf(" ======================== %p\r\n", (void *)((unsigned long) BLOC_MEM(block) + block->dataSize));
        if (block->next != NULL)
        {
            printf("     |              A\r\n");
            printf("     V              |\r\n");
        }
        block = block->next;
        iBloc = iBloc + 1;
    }
    printf("\r\n");
    printf(" Free Size : %d\r\n", freeSize);
    printf(" Sum of Free Blokcs Sizes : %d\r\n", freeSizeSum);
    printf("\r\n");
}
